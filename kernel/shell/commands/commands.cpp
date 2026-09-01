#include "commands.h"
#include "color.h"
#include "print.h"
#include "print_helper.h"
#include "clear.h"
#include "info_text.h"
#include "cmd_history.h"
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

static Return cmd_cd(const Command& command) {
    filesystem_change_directory(command.args[0].value);
    return Return::Success; //TODO HANLDE IN CMD
}

static Return cmd_ls(const Command& command) {
    filesystem_list();
    return Return::Success; //TODO HANLDE IN CMD
}

static Return cmd_pwd(const Command& command) {
    filesystem_print_working_directory();
    return Return::Success; //TODO HANLDE IN CMD
}

static Return cmd_driveinfo(const Command& command) {
    if (selectedStorageDevice->Identified == false) {
        printInfoLine(InfoTextType::Error, "No drive identify data available");
        return Return::Error;
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

    return Return::Success;
}

static Return cmd_dumpsector(const Command& command) {
    if (command.args[0].value[0] == 0) {
        printInfoLine(InfoTextType::Error, "Missing Argument: Sector Number");
        return Return::Error;
    }

    uint8_t buffer[SECTOR_SIZE_BYTES];
    AHCI_READ_DMA_EXT(selectedStorageDevice->Port, to_int(command.args[0].value), 1, buffer);

    printInfoLine(InfoTextType::Info, String(" --- SECTOR ", command.args[0].value, " DATA --- "));
    for (uint64_t i = 0; i < SECTOR_SIZE_BYTES; i++) {
        const char* valueHex = to_string((uint64_t) buffer[i], 16);
        print_inline(valueHex);
        print_inline(" ");
        free(valueHex);
    }
    newline();

    return Return::Success;
}

static Return cmd_read(const Command& command) {
    const char* filePath = str_combine(currentPath, command.args[0].value);

    Entry e = activeDriver->find_entry(filePath);
    if (!e.Found || e.IsDirectory) {
        printInfoLine(InfoTextType::Error, String("File: '", filePath, "' was not found"));
        free(filePath);
        return Return::Error;
    }
    free(filePath);

    uint8_t* data = activeDriver->read(e);
    print((char*) data);
    free(data);

    return Return::Success;
}

static Return cmd_get_uptime(const Command& command) {
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

    return Return::Success;
}

static Return cmd_help(const Command& command) {
    for (uint64_t i = 0; commands[i].name != 0; i++) {
        print_inline(" - ");
        print_inline(commands[i].name);

        if (commands[i].summary[0] != 0) {
            print_inline(" | ");
            print_inline(commands[i].summary);
        }

        if (commands[i].args[0].name != 0) {
            for (uint64_t j = 0; j < MAX_COMMAND_ARGS; j++) {

                if (commands[i].args[j].name == 0) break;

                newline();
                print_inline("   $ ");

                if (commands[i].args[j].isRequired) {
                    print_inline(String("<-", commands[i].args[j].name, ">"));
                } else {
                    print_inline(String("[-", commands[i].args[j].name, "]"));
                }

                switch (commands[i].args[j].type) {
                    case Types::Boolean:
                        print_inline(":Bool");
                        break;
                    case Types::Integer:
                        print_inline(":Int");
                        break;
                    case Types::String:
                        print_inline(":Str");
                        break;
                    default:
                        print_inline(":?");
                        break;
                }

                if (commands[i].args[j].summary[0] != 0) {
                    print_inline(" | ");
                    print_inline(commands[i].args[j].summary);
                }
            }
        }

        newline();
    }

    return Return::Success;
}

static Return cmd_clear(const Command& command) {
    clearScreen();
    cursorAt_X = 0;
    cursorAt_Y = 0;

    cursorRendered_X = 0;
    cursorRendered_Y = 0;

    return Return::Success;
}

static Return cmd_echo(const Command& command) {
    print(command.args[0].value); //TODO only temporary

    return Return::Success;
}

static Return cmd_sysinfo(const Command& command) {
    printSysinfo();

    return Return::Success;
}

static Return cmd_reboot(const Command& command) {
    outb(0x64, 0xFE);

    return Return::NoReturn; //or maybe error
}

static Return cmd_memory_info(const Command& command) {
    print_memory_info();

    return Return::Success;
}

static Return cmd_history(const Command& command) {
    for (uint64_t i = 0; i < cmdHistCount; i++) {
        print(commandHistory[i]);
    }

    return Return::Success;
}

static Return cmd_pciinfo(const Command& command) {
    if (PCIDeviceAmount == 0) {
        printInfoLine(InfoTextType::Error, "No PCI devices were detected on boot");
        return Return::Error;
    }

    const bool compact = str_equal(command.args[0].value, "-c"); //TODO

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

    return Return::Success;
}

Command commands[] = {
    { "help", "", cmd_help },
    { "history", "", cmd_history },
    { "echo", "", cmd_echo,
        Argument("text", "", Types::String, true) },
    { "clear", "", cmd_clear },
    { "sysinfo", "", cmd_sysinfo },
    { "reboot", "", cmd_reboot },
    { "meminfo", "Show memory informations", cmd_memory_info },
    { "uptime", "", cmd_get_uptime },
    { "pciinfo", "List detected PCI devices", cmd_pciinfo,
        Argument("c", "Compact", Types::Boolean, false) },
    { "read", "", cmd_read,
        Argument("file", "", Types::String, true) },
    { "driveinfo", "", cmd_driveinfo },
    { "dumpsector", "Print a sector as hex", cmd_dumpsector,
        Argument("lba", "Sector to dump", Types::Integer, true) },
    { "cd", "", cmd_cd,
        Argument("path", "", Types::String, false), },
    { "ls", "", cmd_ls,
        Argument("path", "", Types::String, false), },
    { "pwd", "", cmd_pwd },
    { 0 }
};
