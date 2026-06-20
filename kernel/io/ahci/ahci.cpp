#include "config.h"
#include <stdint.h>
#include "pci.h"
#include "ahci.h"
#include "paging.h"
#include "info_text.h"

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

void setup_primary_storage_device() {
    map_page(
        (uint64_t)get_virtual_membar_address(find_primary_storage_device().BAR[5]), // virt addr
        get_physical_membar_address(find_primary_storage_device().BAR[5]), // phys addr
        0b00010010,  // writable and cache disabled
        sizeof(AHCI_Registers)
    );

    volatile AHCI_Registers* ahci = (volatile AHCI_Registers*) get_virtual_membar_address(find_primary_storage_device().BAR[5]);

    uint8_t foundAtPort = 0;
    for (uint8_t i = 0; i < 32; i++) {
        if (ahci->Ports[i].SSTS.DET == 3) {
            foundAtPort = i;
            break;
        }
    }

    printInfoLine(InfoTextType::Info, String("Found SATA Storage Medium at Port: ", foundAtPort));
}


void init_ahci() {
    printInfoLine(InfoTextType::Loading, "Loading Primary Storage Device...");
    setup_primary_storage_device();
}
