#include "commands.h"
#include "color.h"
#include "print.h"
#include "print_helper.h"
#include "clear.h"
#include "info_text.h"
#include "terminal.h"
#include "io.h"
#include "timer.h"
#include "pci.h"
#include "pci_class_names.h"
#include "ahci.h"
#include "storage.h"
#include "filesystem.h"
#include "memory_info.h"
#include "heap.h"
#include "integer.h"
#include "string.h"

void cmd_cd(const char* args) {
    filesystem_change_directory(args);
}

void cmd_ls(const char*) {
    filesystem_list();
}

void cmd_pwd(const char*) {
    filesystem_print_working_directory();
}

void cmd_driveinfo(const char*) {
    if (selectedStorageDevice->Identified == false) {
        printInfoLine(InfoTextType::Error, "No drive identify data available");
        return;
    }

    const uint64_t totalGiB = (selectedStorageDevice->IdentificationInformation.AmountOfSectors_64bit * 512) / 1024 / 1024 / 1024;

    print_separator();
    print("----- DRIVE -----");
    printInfoLine(InfoTextType::Info, String("Model:   ", selectedStorageDevice->IdentificationInformation.ModelName));
    printInfoLine(InfoTextType::Info, String("Serial:  ", selectedStorageDevice->IdentificationInformation.SerialNumber));
    printInfoLine(InfoTextType::Info, String("Sectors: ", selectedStorageDevice->IdentificationInformation.AmountOfSectors_64bit));
    printInfoLine(InfoTextType::Info, String("Size:    ", totalGiB, " GiB"));

    newline();
    print("----- ACTIVE PARTITION -----");
    printInfoLine(InfoTextType::Info, String("Start LBA:    ", selectedPartition->Start_LBA));
    printInfoLine(InfoTextType::Info, String("Sector Count: ", selectedPartition->Sector_Count));

    const uint64_t partGiB = ((uint64_t) selectedPartition->Sector_Count * 512) / 1024 / 1024 / 1024;
    printInfoLine(InfoTextType::Info, String("Size:         ", partGiB, " GiB"));

    const char* fsType = "Unknown";
    switch (selectedPartition->Type) {
        case FilesystemType::FAT32:
            fsType = "FAT32";
            break;
        case FilesystemType::FAT16:
            fsType = "FAT16";
            break;
        case FilesystemType::FAT12:
            fsType = "FAT12";
            break;
        case FilesystemType::NTFS:
            fsType = "NTFS";
            break;
        case FilesystemType::Ext:
            fsType = "Ext";
            break;
        default:
            break;
    }
    printInfoLine(InfoTextType::Info, String("Filesystem:   ", fsType));
    printInfoLine(InfoTextType::Info, String("Bootable:     ", selectedPartition->Bootable ? "YES" : "NO"));
    print_separator();
}

void cmd_dumpsector(const char* args) {
    if (args[0] == 0) {
        printInfoLine(InfoTextType::Error, "Missing Argument: Sector Number");
        return;
    }

    uint8_t buffer[SECTOR_SIZE_BYTES];
    AHCI_READ_DMA_EXT(selectedStorageDevice->Port, to_int(args), 1, buffer);

    printInfoLine(InfoTextType::Info, String(" --- SECTOR ", args, " DATA --- "));
    for (uint64_t i = 0; i < SECTOR_SIZE_BYTES; i++) {
        const char* valueHex = to_string((uint64_t) buffer[i], 16);
        print_inline(valueHex);
        print_inline(" ");
        free(valueHex);
    }

    newline();
}

void cmd_read(const char* args) {
    const char* filePath = str_combine(currentPath, args);
    Entry e = activeDriver->find_entry(filePath);
    if (!e.Found || e.IsDirectory) {
        printInfoLine(InfoTextType::Error, String("File: '", filePath, "' was not found"));
        return;
    }
    uint8_t* data = activeDriver->read(e);
    print((char*) data);
    free(data);
}

void cmd_get_uptime(const char*) {
    const uint64_t ms = get_ticks_in_ms();
    const uint64_t sec = ms / 1000;
    const uint64_t min = sec / 60;
    const uint64_t hour = min / 60;
    const uint64_t days = hour / 24;

    print(String(
        days, "d, ",
        hour % 24, "h, ",
        min % 60, "m, ",
        sec % 60, "s, ",
        ms % 1000, "ms"
    ));
}

void cmd_help(const char*) {
    for (uint64_t i = 0; commands[i].name != 0; i++) {
        print_inline(commands[i].name);
        print_inline("; ");
    }
    newline();
}

void cmd_clear(const char*) {
    clearScreen();
    cursorAt_X = 0;
    cursorAt_Y = 0;

    cursorRendered_X = 0;
    cursorRendered_Y = 0;
}

void cmd_echo(const char* args) {
    print(args);
}

void cmd_sysinfo(const char*) {
    printSysinfo();
}

void cmd_reboot(const char*) {
    outb(0x64, 0xFE);
}

void cmd_memory_info(const char*) {
    print_memory_info();
}

void cmd_history(const char*) {
    for (uint64_t i = 0; i < cmdHistCount; i++) {
        print(commandHistory[i]);
    }
}

void cmd_pciinfo(const char* args) {
    if (PCIDeviceAmount == 0) {
        printInfoLine(InfoTextType::Error, "No PCI devices were detected on boot");
        return;
    }

    const bool compact = str_equal(args, "-c");

    printInfoLine(InfoTextType::Info, String(PCIDeviceAmount, " PCI Devices were detected on boot."));

    char* vendorHex;
    char* deviceHex;

    for (uint32_t i = 0; i < PCIDeviceAmount; i++) {
        vendorHex = to_string(found_pci_devices[i].VendorID, 16);
        deviceHex = to_string(found_pci_devices[i].DeviceID, 16);

        const PCIClassInfo classInfo = resolve_class_name(found_pci_devices[i].ClassCode, found_pci_devices[i].Subclass);

        print_inline(" - ");
        print_inline(String("[PCI-", i, "]:"), Color::Yellow);

        if (classInfo.className[0] != 0) {
            print_inline(String(" (", classInfo.className, ")"));
        }
        if (classInfo.subClassName[0] != 0) {
            print_inline(String(" (", classInfo.subClassName, ")"));
        }

        if (compact) {
            print_inline(String(" vID: ", vendorHex, " - dID: ", deviceHex));
        }
        newline();

        if (!compact) {
            print(String("   => Vendor ID: ", vendorHex));
            print(String("   => Device ID: ", deviceHex));
        }

        free(vendorHex);
        free(deviceHex);
    }
}

const Command commands[] = {
    { "help", cmd_help },
    { "history", cmd_history },
    { "echo", cmd_echo },
    { "clear", cmd_clear },
    { "sysinfo", cmd_sysinfo },
    { "reboot", cmd_reboot },
    { "meminfo", cmd_memory_info },
    { "uptime", cmd_get_uptime },
    { "pciinfo", cmd_pciinfo },
    { "read", cmd_read },
    { "driveinfo", cmd_driveinfo },
    { "dumpsector", cmd_dumpsector },
    { "cd", cmd_cd },
    { "ls", cmd_ls },
    { "pwd", cmd_pwd },
    { 0, 0 }
};
