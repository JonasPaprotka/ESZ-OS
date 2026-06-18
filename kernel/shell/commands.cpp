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

void cmd_pciinfo(const char*) {
    if (PCIDeviceAmount == 0) {
        printInfoLine(InfoTextType::Error, "No PCI devices were detected on boot");
        return;
    }

    printInfoLine(InfoTextType::Info, String(PCIDeviceAmount, " PCI Devices were detected on boot."));

    char* hexString;
    for (uint32_t i = 0; i < PCIDeviceAmount; i++) {
        print(String(" - [Device ", i, "]:"), Color::Yellow);

        hexString = to_string(found_pci_devices[i].VendorID, 16);
        print(String("   => Vendor ID: ", hexString));
        free(hexString);

        hexString = to_string(found_pci_devices[i].DeviceID, 16);
        print(String("   => Device ID: ", hexString));
        free(hexString);
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
    { 0, 0 }
};
