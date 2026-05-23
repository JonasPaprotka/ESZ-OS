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
    print("--------------------- ", DarkGray);
    print("[INFO]: Kernel Loaded", Yellow);
    print("ESZ-OS");
    print("by Jonas Paprotka");
    print("--------------------- ", DarkGray);
}

int cursorAtChar = 0;
int cursorAtLine = 0;

void terminal_on_key(unsigned char scancode) {
    char c = scancode_to_ascii[scancode];
    if (!c) { return; }
    print_char(c);
    ++cursorAtChar;
}

void newTerminalLine() {
    cursorAtChar = 0;
    print(">> ", White, cursorAtLine, 0);
    cursorAtChar = 3;
}

void terminal_init() {
    set_cursor(0);
    printHeader();
    newTerminalLine();
}
