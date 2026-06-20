#include "config.h"
#include "pci.h"
#include "info_text.h"
#include "memory.h"
#include "io.h"
#include "string.h"
#include "ahci.h"
#include "paging.h"

PCI_Device* found_pci_devices;
uint32_t PCIDeviceAmount = 0;

const uint32_t PCI_ENUM_WRITE_PORT = 0xCF8;
const uint32_t PCI_ENUM_READ_PORT = 0xCFC;
const uint32_t START_DEVICE_ADDRESS = 0x80000000;

uint32_t calc_address(const uint16_t bus, const uint8_t device, const uint8_t funct) {
    return 0x80000000 | (bus << 16) | (device << 11) | (funct << 8);
}

uint32_t pci_read(const uint16_t bus, const uint8_t dev, const uint8_t func, const uint8_t offset) {
    outl(PCI_ENUM_WRITE_PORT, calc_address(bus, dev, func) | (offset & 0xFC));
    return inl(PCI_ENUM_READ_PORT);
}

bool is_device_present(const uint32_t value) {
    return (value & 0xFFFF) != 0xFFFF;
}

uint32_t count_pci_devices() {
    uint32_t deviceCounter = 0;

    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            for (uint8_t funct = 0; funct < 8; funct++) {
                outl(PCI_ENUM_WRITE_PORT, calc_address(bus, device, funct));
                if (is_device_present(inl(PCI_ENUM_READ_PORT))) {
                    deviceCounter++;
                }
            }
        }
    }

    return deviceCounter;
}

bool is_IO_BAR(const PCI_BAR bar) {
    return bar.io.RegionType == 1;
}

void get_pci_devices() {
    uint32_t deviceCounter = 0;

    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            for (uint8_t funct = 0; funct < 8; funct++) {
                const uint32_t reg0 = pci_read(bus, device, funct, 0x04 * 0);
                if (!is_device_present(reg0)) continue;
                found_pci_devices[deviceCounter].VendorID = (uint16_t)(reg0 & 0xFFFF);
                found_pci_devices[deviceCounter].DeviceID = (uint16_t)(reg0 >> 16);

                const uint32_t reg1 = pci_read(bus, device, funct, 0x04 * 1);
                found_pci_devices[deviceCounter].Command = (uint16_t)(reg1 & 0xFFFF);
                found_pci_devices[deviceCounter].Status = (uint16_t)(reg1 >> 16);

                const uint32_t reg2 = pci_read(bus, device, funct, 0x04 * 2);
                found_pci_devices[deviceCounter].RevisionID = (uint8_t)(reg2 & 0xFF);
                found_pci_devices[deviceCounter].ProgIF = (uint8_t)(reg2 >> 8);
                found_pci_devices[deviceCounter].Subclass = (uint8_t)(reg2 >> 16);
                found_pci_devices[deviceCounter].ClassCode = (uint8_t)(reg2 >> 24);

                const uint32_t reg3 = pci_read(bus, device, funct, 0x04 * 3);
                found_pci_devices[deviceCounter].CacheLineSize = (uint8_t)(reg3 & 0xFFFF);
                found_pci_devices[deviceCounter].LatencyTimer = (uint8_t)(reg3 >> 8);
                found_pci_devices[deviceCounter].HeaderType = (uint8_t)(reg3 >> 16);
                found_pci_devices[deviceCounter].BIST.CompetionCode = (uint8_t)((reg3 >> 24) & 0b00001111);
                found_pci_devices[deviceCounter].BIST.Reserved = (uint8_t)((reg3 >> 24) & 0b00110000) >> 4;
                found_pci_devices[deviceCounter].BIST.Start_BIST = (uint8_t)((reg3 >> 24) & 0b01000000) >> 6;
                found_pci_devices[deviceCounter].BIST.BIST_Capable = (uint8_t)((reg3 >> 24) & 0b10000000) >> 7;

                // reg4 - reg9
                for (uint8_t i = 0; i < 6; i++) {
                    const uint32_t val = pci_read(bus, device, funct, 0x10 + (i * 4));
                    found_pci_devices[deviceCounter].BAR[i].raw = val;
                }

                const uint32_t reg10 = pci_read(bus, device, funct, 0x04 * 10);
                found_pci_devices[deviceCounter].CardbusCISPointer = (uint32_t) reg10;

                const uint32_t reg11 = pci_read(bus, device, funct, 0x04 * 11);
                found_pci_devices[deviceCounter].SubsystemVendorID = (uint16_t)(reg11 & 0xFFFF);
                found_pci_devices[deviceCounter].SubsystemID = (uint16_t)(reg11 >> 16);

                const uint32_t reg12 = pci_read(bus, device, funct, 0x04 * 12);
                found_pci_devices[deviceCounter].ExpansionROMBaseAddress = (uint32_t) reg12;

                const uint32_t reg13 = pci_read(bus, device, funct, 0x04 * 13);
                found_pci_devices[deviceCounter].CapabilitiesPointer = (uint8_t)(reg13 & 0xFFFF);
                // found_pci_devices[deviceCounter].Reserved

                //const uint32_t reg14 = pci_read(bus, device, funct, 0x04 * 14);
                // found_pci_devices[deviceCounter].Reserved

                const uint32_t reg15 = pci_read(bus, device, funct, 0x04 * 15);
                found_pci_devices[deviceCounter].InterruptLine = (uint8_t)(reg15 & 0xFFFF);
                found_pci_devices[deviceCounter].InterruptPin = (uint8_t)(reg15 >> 8);
                found_pci_devices[deviceCounter].MinGnt = (uint8_t)(reg15 >> 16);
                found_pci_devices[deviceCounter].MaxLat = (uint8_t)(reg15 >> 24);

                deviceCounter++;
            }
        }
    }
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

void init_pci() {
    printInfoLine(InfoTextType::Loading, "Loading PCI Devices...");
    PCIDeviceAmount = count_pci_devices();
    found_pci_devices = (PCI_Device*) malloc(sizeof(PCI_Device) * PCIDeviceAmount);
    get_pci_devices();
    printInfoLine(InfoTextType::Success, String("Loaded ", PCIDeviceAmount, " PCI Devices"));

    map_page(
        (uint64_t)get_virtual_membar_address(find_primary_storage_device().BAR[5]), // virt addr
        get_physical_membar_address(find_primary_storage_device().BAR[5]), // phys addr
        0b00010010  // writable and cache disabled
    );

    // printInfoLine(InfoTextType::Debug, to_string((uint64_t)get_virtual_membar_address(find_primary_storage_device().BAR[5]), 16));
    volatile AHCI_Registers* ahci = (volatile AHCI_Registers*) get_virtual_membar_address(find_primary_storage_device().BAR[5]);
}
