#include "string.h"
#include "print.h"
#include "commands.h"
#include "terminal/terminal.h"
#include "io/io.h"
#include "memory.h"
#include "clear.h"

void cmd_help(const char*) {
    for (int i = 0; commands[i].name != 0; i++) {
        print_inline(commands[i].name);
        print_inline("; ");
    }
    newline();
}

void cmd_clear(const char*) {
    clearScreen();
}

void cmd_echo(const char* args) {
    print(args);
}

void cmd_os_info(const char*) {
    print("ESZ-OS (W.I.P.) by Jonas Paprotka");
}

void cmd_reboot(const char*) {
    outb(0x64, 0xFE);
}

void cmd_memory_info(const char*) {
    //printMemoryInfo();
}

const Command commands[] = {
    { "help", cmd_help },
    { "echo", cmd_echo },
    { "clear", cmd_clear },
    { "os-info", cmd_os_info },
    { "reboot", cmd_reboot },
    { "memory-info", cmd_memory_info },
    { 0, 0 }
};
