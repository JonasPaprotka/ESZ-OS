#include "io.h"
#include "pic.h"
#include "vga.h"

char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

int cursorAtChar = 0;
int cursorAtLine = 5;

void keyboard_handler() {
    unsigned char scancode = inb(0x60);

    if (scancode < 58 && scancode_to_ascii[scancode]) {
        print_char(scancode_to_ascii[scancode], White, 80 * cursorAtLine + cursorAtChar);
        ++cursorAtChar;
    } else {
        // Backspace
        if (scancode == 0x0E) {
            if (cursorAtChar <= 0) { return; }
            --cursorAtChar;
            print_char(' ', White, 80 * cursorAtLine + cursorAtChar);
        }
    }

    outb(0x20, 0x20);
}
