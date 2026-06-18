#include "pci.h"
#include "info_text.h"
#include "memory.h"
#include "io.h"
#include "string.h"

PCI_Device* found_pci_devices;

const uint32_t PCI_ENUM_WRITE_PORT = 0xCF8;
const uint32_t PCI_ENUM_READ_PORT = 0xCFC;
const uint32_t START_DEVICE_ADDRESS = 0x80000000;

uint32_t calc_address(const uint16_t bus, const uint8_t device, const uint8_t funct) {
    return 0x80000000 | (bus << 16) | (device << 11) | (funct << 8);
}

bool is_device_present(const uint32_t value) {
    return (value & 0xFFFF) != 0xFFFF;
}

uint32_t count_pci_devices() {
    uint32_t currentDevice = 0;
    uint32_t deviceCounter = 0;

    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            for (uint8_t funct = 0; funct < 8; funct++) {
                outl(PCI_ENUM_WRITE_PORT, calc_address(bus, device, funct));
                if (!is_device_present(inl(PCI_ENUM_READ_PORT))) deviceCounter++;
            }
        }
    }

    return deviceCounter;
}

void get_pci_devices() {
    PCI_Device currentDevice;
    uint32_t deviceCounter = 0;

    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            for (uint8_t funct = 0; funct < 8; funct++) {
                outl(PCI_ENUM_WRITE_PORT, calc_address(bus, device, funct));
                const uint32_t value = inl(PCI_ENUM_READ_PORT);
                if (!is_device_present(value)) continue;

                found_pci_devices[deviceCounter].VendorID = (uint16_t)(value & 0xFFFF);
                found_pci_devices[deviceCounter].DeviceID = (uint16_t)(value >> 16) & 0xFFFF;

                printInfoLine(InfoTextType::Debug, String("VendorID: ", to_string((uint64_t) found_pci_devices[deviceCounter].VendorID, 16), " - DeviceID: ", to_string((uint64_t)found_pci_devices[deviceCounter].DeviceID, 16)));

                deviceCounter++;
            }
        }
    }
}

void init_pci() {
    printInfoLine(InfoTextType::Loading, "PCI...");
    found_pci_devices = (PCI_Device*) malloc(sizeof(PCI_Device) * count_pci_devices());
    get_pci_devices();
}
