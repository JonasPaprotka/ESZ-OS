#include "terminal.h"
#include "vga.h"
#include "keyboard.h"

char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

void printHeader() {
    printf("--------------------- ", DarkGray, 0, 0);
    printf("[INFO]: ", Yellow, 1, 0);
    printf("Kernel Loaded", White, 1, 8);
    printf("ESZ-OS", White, 2, 0);
    printf("by Jonas Paprotka", White, 3, 0);
    printf("--------------------- ", DarkGray, 4, 0);
}

int cursorAtChar = 0;
int cursorAtLine = 0;

void terminal_on_key(unsigned char scancode) {
    print_char(scancode_to_ascii[scancode], White, 80 * cursorAtLine + cursorAtChar);
    ++cursorAtChar;
}

void newTerminalLine() {
    //printf(">> ", White,);
}

void terminal_init() {
    set_cursor(0);
    printHeader();
    newTerminalLine();
}
