#include "config.h"
#include <stdint.h>
#include "pci.h"
#include "ahci.h"
#include "paging.h"
#include "info_text.h"
#include "timer.h"
#include "memory.h"
#include "math.h"

PCI_Device* ahci_controller;
PCI_BAR BAR_IDX_5;

void Start_AHCI_Command(volatile AHCI_Ports* port) {
    port->CI |= (1 << 0);
}

bool Await_AHCI_Task_Finish(volatile AHCI_Ports* port) {
    const uint64_t timeout = 3000; // 3 sec
    uint64_t elapsed = 0;

    while (port->CI & (1 << 0)) {
        sleep_ms(1);
        if (++elapsed >= timeout) return false; // timeout
    }
    return true;
}

void ata_string_byteswap(char* text, const uint32_t length) {
    for (uint32_t i = 0; i < length; i += 2) {
        char temp = text[i];
        text[i] = text[i + 1];
        text[i + 1] = temp;
    }
}

bool await_port_status_change(volatile AHCI_Ports* port, const bool active) {
    const uint64_t timeout = 3000; // 3 sec
    uint64_t elapsed = 0;
    const uint8_t pollStatus = active;

    while (port->CMD.CR != pollStatus || port->CMD.FR != pollStatus) {
        sleep_ms(1);
        if (++elapsed >= timeout) return false; // timeout
    }
    return true;
}

bool await_drive_ready(volatile AHCI_Ports* port) {
    const uint64_t timeout = 3000; // 3 sec
    uint64_t elapsed = 0;

    while (port->TFD.STS.BSY == 1 || port->TFD.STS.DRDY == 0) {
        sleep_ms(1);
        if (++elapsed >= timeout) return false; // timeout
    }
    return true;
}

bool set_sata_port_status(volatile AHCI_Ports* port, const bool activate) {
    if (activate) {
        port->CMD.ST = 1;
        port->CMD.FRE = 1;
        return await_port_status_change(port, true);
    }

    port->CMD.ST = 0;
    port->CMD.FRE = 0;
    return await_port_status_change(port, false);
}

volatile uint32_t* get_virtual_membar_address(const PCI_BAR bar) {
    return (uint32_t*)((uint32_t)(bar.mem.BaseAddress << 4) + VIRTUAL_OFFSET_MMIO);
}

uint64_t get_physical_membar_address(const PCI_BAR bar) {
    return (uint32_t)(bar.mem.BaseAddress << 4);
}

PCI_Device* find_ahci_controller() {
    for (uint32_t i = 0; i < PCIDeviceAmount; i++) {
        if ((found_pci_devices[i].ClassCode == 0x1) && (found_pci_devices[i].Subclass == 0x6)) {
            return &found_pci_devices[i];
        }
    }
    return nullptr;
}

uint64_t AllocateCommandTable() {
    uint64_t commandTablePhysAddr = pmm_malloc_page();
    memory_clear((void*)(commandTablePhysAddr + hhdm_offset), PAGE_SIZE);
    return commandTablePhysAddr;
}

uint64_t AllocateDataBuffer(const uint32_t dataSize) {
    const uint64_t requiredPages = divide_round_up(dataSize, PAGE_SIZE);
    uint64_t dataBufferPhysAddr = pmm_malloc_pages(requiredPages);
    memory_clear((void*)(dataBufferPhysAddr + hhdm_offset), PAGE_SIZE * requiredPages);
    return dataBufferPhysAddr;
}

AHCI_Command_Header* SetCommandHeader(volatile AHCI_Ports* port, const uint64_t commandTablePhysAddr, const bool write) {
    AHCI_Command_Header* commandList = (AHCI_Command_Header*)(port->CLB + hhdm_offset);
    commandList[0].CTBA = (uint32_t) commandTablePhysAddr;
    commandList[0].CTBAU = 0;
    commandList[0].CFL = 5; // size in 4-byte blocks - H2D-FIS is 5
    commandList[0].W = write;
    commandList[0].PRDTL = 1; // amount PRDTL entries for command

    return commandList;
}

AHCI_Command_Table* BuildCommand(const uint64_t commandTablePhysAddr, const uint8_t CommandCode) {
    AHCI_Command_Table* commandTable = (AHCI_Command_Table*)(commandTablePhysAddr + hhdm_offset);
    commandTable->CFIS.FISType = 0x27; // marker for H2D_Register_FIS
    commandTable->CFIS.C = 1; // 1=real command
    commandTable->CFIS.Command = CommandCode;

    return commandTable;
}

void Fill_LBA_Address(AHCI_Command_Table* commandTable, const uint64_t LBA_Address) {
    commandTable->CFIS.Device = 0x40; // LBA mode

    // split LBA address in lower 6 bytes
    commandTable->CFIS.LBA0 = LBA_Address;
    commandTable->CFIS.LBA1 = LBA_Address >> 8;
    commandTable->CFIS.LBA2 = LBA_Address >> 16;
    commandTable->CFIS.LBA3 = LBA_Address >> 24;
    commandTable->CFIS.LBA4 = LBA_Address >> 32;
    commandTable->CFIS.LBA5 = LBA_Address >> 40;
}

void Fill_Sector_Quantity(AHCI_Command_Table* commandTable, const uint16_t sectorQuantity) {
    commandTable->CFIS.CountLow = sectorQuantity;
    commandTable->CFIS.CountHigh = sectorQuantity >> 8;
}

IDENTIFY_Response* Get_AHCI_IdentifyResponse(const uint64_t dataBufferPhysAddr) {
    IDENTIFY_Response* identifyData = (IDENTIFY_Response*)(dataBufferPhysAddr + hhdm_offset);

    identifyData->Reserved2[0] = 0;
    identifyData->Reserved3[0] = 0;

    ata_string_byteswap(identifyData->SerialNumber, 20);
    char* serialNumberValue = str_trim_end(identifyData->SerialNumber);
    str_copy(identifyData->SerialNumber, serialNumberValue);
    free(serialNumberValue);

    ata_string_byteswap(identifyData->ModelName, 40);
    char* modelNameValue = str_trim_end(identifyData->ModelName);
    str_copy(identifyData->ModelName, modelNameValue);
    free(modelNameValue);

    return identifyData;
}

void set_ahci_prdt(AHCI_Command_Table* commandTable, const uint64_t dataBufferPhysAddr, const uint32_t dataSize) {
    commandTable->PRDT[0].DBA = (uint32_t) dataBufferPhysAddr; // input here
    commandTable->PRDT[0].DBAU = 0;
    commandTable->PRDT[0].DBC = dataSize - 1;
}

bool RunCommand(volatile AHCI_Ports* port) {
    Start_AHCI_Command(port);
    return Await_AHCI_Task_Finish(port);
}

IDENTIFY_Response* AHCI_IDENTIFY_DEVICE(volatile AHCI_Ports* port) {
    uint64_t commandTablePhysAddr = AllocateCommandTable();
    uint64_t dataBufferPhysAddr = AllocateDataBuffer(PAGE_SIZE);

    SetCommandHeader(port, commandTablePhysAddr, false);
    AHCI_Command_Table* commandTable = BuildCommand(commandTablePhysAddr, IDENTIFY_DEVICE);

    // CONFIGURE OUTPUT
    set_ahci_prdt(commandTable, dataBufferPhysAddr, SECTOR_SIZE_BYTES);

    if (!RunCommand(port)) return nullptr;

    return Get_AHCI_IdentifyResponse(dataBufferPhysAddr);
}

void AHCI_WRTIE_DMA_EXT(volatile AHCI_Ports* port, const uint64_t writeStartLBA, const uint16_t sectorQuantity, const void* RAM_InputPtr) {
    uint64_t commandTablePhysAddr = AllocateCommandTable();
    const uint32_t dataSize = sectorQuantity * SECTOR_SIZE_BYTES;
    uint64_t dataBufferPhysAddr = AllocateDataBuffer(dataSize);

    SetCommandHeader(port, commandTablePhysAddr, true);
    AHCI_Command_Table* commandTable = BuildCommand(commandTablePhysAddr, WRITE_DMA_EXT);

    Fill_LBA_Address(commandTable, writeStartLBA);
    Fill_Sector_Quantity(commandTable, sectorQuantity);

    // FILL INPUT DATA
    memory_copy((uint64_t*)(dataBufferPhysAddr + hhdm_offset), RAM_InputPtr, dataSize);

    // CONFIGURE INPUT
    set_ahci_prdt(commandTable, dataBufferPhysAddr, dataSize);

    RunCommand(port);
}

void AHCI_READ_DMA_EXT(volatile AHCI_Ports* port, const uint64_t readStartLBA, const uint16_t sectorQuantity, void* RAM_OutputPtr) {
    uint64_t commandTablePhysAddr = AllocateCommandTable();
    const uint32_t dataSize = sectorQuantity * SECTOR_SIZE_BYTES;
    uint64_t dataBufferPhysAddr = AllocateDataBuffer(dataSize);

    SetCommandHeader(port, commandTablePhysAddr, false);
    AHCI_Command_Table* commandTable = BuildCommand(commandTablePhysAddr, READ_DMA_EXT);

    Fill_LBA_Address(commandTable, readStartLBA);
    Fill_Sector_Quantity(commandTable, sectorQuantity);

    // CONFIGURE OUTPUT
    set_ahci_prdt(commandTable, dataBufferPhysAddr, dataSize);

    RunCommand(port);

    // GET DATA
    memory_copy(RAM_OutputPtr, (void*)(dataBufferPhysAddr + hhdm_offset), dataSize);
}

void AHCI_FLUSH_CACHE_EXT(volatile AHCI_Ports* port) {
    uint64_t commandTablePhysAddr = AllocateCommandTable();

    AHCI_Command_Header* commandList = SetCommandHeader(port, commandTablePhysAddr, false);
    commandList[0].PRDTL = 0;
    BuildCommand(commandTablePhysAddr, FLUSH_EXT);

    RunCommand(port);
}

bool preparePort(volatile AHCI_Ports* port) {
    // shutdown port
    if (!set_sata_port_status(port, false)) return false;

    port->CLB = pmm_malloc_page(); // 1024 bytes needed
    memory_clear((void*)(port->CLB + hhdm_offset), PAGE_SIZE);
    port->CLBU = 0;
    port->FB = pmm_malloc_page(); // 256 bytes needed
    memory_clear((void*)(port->FB + hhdm_offset), PAGE_SIZE);
    port->FBU = 0;

    // start port
    if (!set_sata_port_status(port, true)) return false;

    return await_drive_ready(port);
}

bool await_port_reset(volatile AHCI_Ports* port) {
    const uint64_t timeout = 3000;
    uint64_t elapsed = 0;
    while ((port->SSTS.DET != 3)) {
        sleep_ms(1);
        if (++elapsed >= timeout) return false;
    }
    return true;
}

bool reset_port_link(volatile AHCI_Ports* port) {
    port->SCTL = (port->SCTL & ~0xF) | 0x1; // DET=1 - COMRESET
    sleep_ms(10);
    port->SCTL = (port->SCTL & ~0xF) | 0x0; // DET=0 - Free

    return await_port_reset(port);
}

bool find_ready_ports(ReadyPort outReadyPorts[], uint8_t &outReadyPortCount) {
    map_pages(
        (uint64_t)get_virtual_membar_address(BAR_IDX_5), // virt addr
        get_physical_membar_address(BAR_IDX_5), // phys addr
        0b00010010,  // writable and cache disabled
        sizeof(AHCI_Registers)
    );

    volatile AHCI_Registers* ahciVirtualRegisters = (volatile AHCI_Registers*) get_virtual_membar_address(BAR_IDX_5);

    for (uint8_t i = 0; i < 32; i++) {
        if (!(ahciVirtualRegisters->PI & (1 << i))) continue;

        if (ahciVirtualRegisters->Ports[i].SSTS.DET == 1) reset_port_link(&ahciVirtualRegisters->Ports[i]);
        if (ahciVirtualRegisters->Ports[i].SSTS.DET != 3) continue;

        volatile AHCI_Ports* candidate = &ahciVirtualRegisters->Ports[i];

        if (!preparePort(candidate)) continue;
        if (candidate->SIG != 0x00000101) continue; // only normal SATA

        outReadyPorts[outReadyPortCount].PortPointer = candidate;
        outReadyPorts[outReadyPortCount].PortIndex = i;
        outReadyPortCount++;
    }

    return outReadyPortCount > 0;
}

bool init_ahci() {
    // TODO fix for multiple ahci controllers
    ahci_controller = find_ahci_controller();
    if (ahci_controller == nullptr) return false;
    BAR_IDX_5 = ahci_controller->BAR[5];

    return true;
}
