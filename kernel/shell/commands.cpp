#include "config.h"
#include "string.h"
#include "print.h"
#include "commands.h"
#include "terminal/terminal.h"
#include "io/io.h"
#include "memory.h"
#include "clear.h"
#include "info_text.h"

void cmd_help(const char*) {
    for (int i = 0; commands[i].name != 0; i++) {
        print_inline(commands[i].name);
        print_inline("; ");
    }
    newline();
}

void cmd_clear(const char*) {
    clearScreen();
    cursorAt_X = 0;
    cursorAt_Y = 0;
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

const Command commands[] = {
    { "help", cmd_help },
    { "echo", cmd_echo },
    { "clear", cmd_clear },
    { "sysinfo", cmd_sysinfo },
    { "reboot", cmd_reboot },
    { "meminfo", cmd_memory_info },
    { 0, 0 }
};
