#include "config.h"
#include <stdint.h>
#include "pci.h"
#include "ahci.h"
#include "paging.h"
#include "info_text.h"
#include "timer.h"
#include "memory.h"

volatile AHCI_Registers* ahci;
volatile AHCI_Ports* foundPortPtr = nullptr;

void ata_string_byteswap(char* text, const uint32_t length) {
    for (uint32_t i = 0; i < length; i += 2) {
        char temp = text[i];
        text[i] = text[i + 1];
        text[i + 1] = temp;
    }
}

void await_port_status_change(volatile AHCI_Ports* port, const bool active) {
    const uint8_t pollStatus = active;
    while (port->CMD.CR != pollStatus || port->CMD.FR != pollStatus) {
        sleep_ms(1);
    }
}

void set_sata_port_status(volatile AHCI_Ports* port, const bool activate) {
    if (activate) {
        port->CMD.ST = 1;
        port->CMD.FRE = 1;
        await_port_status_change(port, true);
        return;
    }

    port->CMD.ST = 0;
    port->CMD.FRE = 0;
    await_port_status_change(port, false);
}

volatile uint32_t* get_virtual_membar_address(const PCI_BAR bar) {
    return (uint32_t*)((uint32_t)(bar.mem.BaseAddress << 4) + VIRTUAL_OFFSET_MMIO);
}

uint64_t get_physical_membar_address(const PCI_BAR bar) {
    return (uint32_t)(bar.mem.BaseAddress << 4);
}

PCI_Device find_primary_storage_device() {
    for (uint32_t i = 0; i < PCIDeviceAmount; i++) {
        if ((found_pci_devices[i].ClassCode == 0x1) && (found_pci_devices[i].Subclass == 0x6)) {
            return found_pci_devices[i];
        }
    }
    PCI_Device emtpyDevice;
    return emtpyDevice;
}

void ahci_identify_device(volatile AHCI_Ports* port) {
    uint64_t dataBufferPhysAddr = pmm_malloc_page();
    memory_clear((void*)(dataBufferPhysAddr + hhdm_offset), PAGE_SIZE);

    uint64_t commandTablePhysAddr = pmm_malloc_page();
    memory_clear((void*)(commandTablePhysAddr + hhdm_offset), PAGE_SIZE);

    // assign command header -> point to command table
    AHCI_Command_Header* commandList = (AHCI_Command_Header*)(port->CLB + hhdm_offset);
    commandList[0].CTBA = (uint32_t) commandTablePhysAddr;
    commandList[0].CTBAU = 0;
    commandList[0].CFL = 5; // size in 4-byte blocks - H2D-FIS is 5
    commandList[0].W = 0; // 0=read
    commandList[0].PRDTL = 1; // amount PRDTL entries for command

    // IDENTIFY command
    AHCI_Command_Table* commandTable = (AHCI_Command_Table*)(commandTablePhysAddr + hhdm_offset);
    commandTable->CFIS.FISType = 0x27; // marker for H2D_Register_FIS
    commandTable->CFIS.C = 1; // 1=real command
    commandTable->CFIS.Command = 0xEC; // IDENTIFY DEVICE

    // plan output
    commandTable->PRDT[0].DBA = (uint32_t) dataBufferPhysAddr; // output here
    commandTable->PRDT[0].DBAU = 0;
    commandTable->PRDT[0].DBC = 511; // 512 byte output length


    // set command slot0 to ready
    port->CI |= (1 << 0);

    // await task finished
    while (port->CI & (1 << 0)) {
        sleep_ms(1);
    }

    IDENTIFY_Response* identifyData = (IDENTIFY_Response*)(dataBufferPhysAddr + hhdm_offset);
    identifyData->Reserved2[0] = 0;
    identifyData->Reserved3[0] = 0;

    ata_string_byteswap(identifyData->SerialNumber, 20);
    str_copy(identifyData->SerialNumber, str_trim_end(identifyData->SerialNumber));

    ata_string_byteswap(identifyData->ModelName, 40);
    str_copy(identifyData->ModelName, str_trim_end(identifyData->ModelName));

    printInfoLine(InfoTextType::Info, String("Model Name: ", identifyData->ModelName));
    printInfoLine(InfoTextType::Info, String("Serial Number: ", identifyData->SerialNumber));
    printInfoLine(InfoTextType::Info, String("Amount of Sectors: ", identifyData->AmountOfSectors_64bit));
}

void search_available_port() {
    map_pages(
        (uint64_t)get_virtual_membar_address(find_primary_storage_device().BAR[5]), // virt addr
        get_physical_membar_address(find_primary_storage_device().BAR[5]), // phys addr
        0b00010010,  // writable and cache disabled
        sizeof(AHCI_Registers)
    );

    ahci = (volatile AHCI_Registers*) get_virtual_membar_address(find_primary_storage_device().BAR[5]);

    uint8_t foundAtPort = 0;
    for (uint8_t i = 0; i < 32; i++) {
        if (ahci->Ports[i].SSTS.DET == 3 && ahci->Ports[i].SIG == 0x00000101) { // available and ATA
            foundAtPort = i;
            foundPortPtr = &ahci->Ports[foundAtPort];
            break;
        }
    }

    if (foundPortPtr == nullptr) {
        printInfoLine(InfoTextType::Error, "No Primary SATA Storage Medium was found.");
        return;
    }

    printInfoLine(InfoTextType::Info, String("Found SATA Storage Medium at Port: ", foundAtPort));
}

void setup_primary_storage_device() {
    search_available_port();

    // ------------- PREPARE PORT -------------
    set_sata_port_status(foundPortPtr, false); // shutdown port

    foundPortPtr->CLB = pmm_malloc_page(); // 1024 bytes needed
    memory_clear((void*)(foundPortPtr->CLB + hhdm_offset), PAGE_SIZE);
    foundPortPtr->CLBU = 0;

    foundPortPtr->FB = pmm_malloc_page(); // 256 bytes needed
    memory_clear((void*)(foundPortPtr->FB + hhdm_offset), PAGE_SIZE);
    foundPortPtr->FBU = 0;

    set_sata_port_status(foundPortPtr, true); // start port
    // -----------------------------------------

}


void init_ahci() {
    printInfoLine(InfoTextType::Loading, "Loading Primary Storage Device...");

    setup_primary_storage_device();
    ahci_identify_device(foundPortPtr);
}
