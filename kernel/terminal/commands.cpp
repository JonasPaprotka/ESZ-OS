#include "string.h"
#include "vga.h"
#include "commands.h"
#include "terminal.h"

void cmd_clear(const char* args) {
    clear();
}

void cmd_echo(const char* args) {
    print(args);
}

Command commands[] = {
    { "echo", cmd_echo },
    { "clear", cmd_clear },
    { 0, 0 }
};
