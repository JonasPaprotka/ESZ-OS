#include "terminal.h"
#include "vga.h"
#include "keyboard.h"
#include "string.h"

char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

void printHeader() {
    print("---------------------- ", DarkGray);
    print("[INFO]: Kernel Loaded", Green);
    print("[WARN]: Kernel W.I.P.", Yellow);
    print("ESZ-OS");
    print("by Jonas Paprotka");
    print("---------------------- ", DarkGray);
}

int cursorAtChar = 0;
int cursorAtLine = 0;
int charsProtectedTil = 0;

void newTerminalLine() {
    ++cursorAtLine;
    cursorAtChar = 0;
    const string linePrefix = "root@esz >> ";
    print(linePrefix, White, cursorAtLine, 0);
    cursorAtChar = strlen(linePrefix);
    charsProtectedTil = cursorAtChar;
}

void terminal_on_key(unsigned char scancode) {
    switch(scancode) {
        case 0x1C: // Enter
            // TODO check line input and stuff
            newTerminalLine();
            break;
        case 0x0E: // Backspace
            if (cursorAtChar == charsProtectedTil) { break; }
            --cursorAtChar;
            clear_char(80 * cursorAtLine + cursorAtChar);
            break;
        default:
            char c = scancode_to_ascii[scancode];
            if (!c) return;
            print_char(c);
            ++cursorAtChar;
            break;
    }
}

void terminal_init() {
    printHeader();
    newTerminalLine();
}
