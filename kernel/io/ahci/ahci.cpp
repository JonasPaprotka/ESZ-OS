#include "config.h"
#include <stdint.h>
#include "pci.h"
#include "ahci.h"
#include "paging.h"
#include "info_text.h"
#include "timer.h"

volatile AHCI_Registers* ahci;

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

void setup_primary_storage_device() {
    map_pages(
        (uint64_t)get_virtual_membar_address(find_primary_storage_device().BAR[5]), // virt addr
        get_physical_membar_address(find_primary_storage_device().BAR[5]), // phys addr
        0b00010010,  // writable and cache disabled
        sizeof(AHCI_Registers)
    );

    ahci = (volatile AHCI_Registers*) get_virtual_membar_address(find_primary_storage_device().BAR[5]);

    uint8_t foundAtPort = 0;
    volatile AHCI_Ports* foundPortPtr = nullptr;
    for (uint8_t i = 0; i < 32; i++) {
        if (ahci->Ports[i].SSTS.DET == 3) {
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

    // shutdown port
    set_sata_port_status(foundPortPtr, false);
}


void init_ahci() {
    printInfoLine(InfoTextType::Loading, "Loading Primary Storage Device...");
    setup_primary_storage_device();
}
