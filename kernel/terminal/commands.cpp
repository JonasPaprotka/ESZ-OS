#include "string.h"
#include "vga.h"
#include "commands.h"
#include "terminal.h"
#include "io.h"

void cmd_help(const char* args) {
    for (int i = 0; commands[i].name != 0; i++) {
        print_inline(commands[i].name);
        print_inline("; ");
    }
    newline();
}

void cmd_clear(const char* args) {
    clear();
}

void cmd_echo(const char* args) {
    print(args);
}

void cmd_os_info(const char* args) {
    print("ESZ-OS (W.I.P.) by Jonas Paprotka");
}

void cmd_reboot(const char* args) {
    outb(0x64, 0xFE);
}

Command commands[] = {
    { "help", cmd_help },
    { "echo", cmd_echo },
    { "clear", cmd_clear },
    { "os-info", cmd_os_info },
    { "reboot", cmd_reboot },
    { 0, 0 }
};
