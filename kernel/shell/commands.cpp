#include "config.h"
#include "string.h"
#include "print.h"
#include "commands.h"
#include "terminal.h"
#include "io.h"
#include "memory.h"
#include "clear.h"
#include "info_text.h"
#include "terminal.h"
#include "timer.h"
#include "pci.h"
#include "pci_class_names.h"
#include "fat32.h"

void cmd_read_file(const char* args) {
    uint8_t* fileData = read_file(args);
    if (fileData == nullptr) return;
    print((char*) fileData);
    free(fileData);
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
    printInfoLine(InfoTextType::Info, String(OS_NAME, " - ", ARCH_NAME, " - (", OS_VERSION_STRING, ") by ", OS_AUTHOR));
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
    { "read-file", cmd_read_file },
    { 0, 0 }
};
