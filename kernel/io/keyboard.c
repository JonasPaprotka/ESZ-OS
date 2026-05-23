#include "io.h"
#include "pic.h"
#include "vga.h"
#include "terminal.h"

extern bool shift = false;

void keyboard_handler() {
    unsigned char scancode = inb(0x60);

    if (scancode == 0x2A || scancode == 0x36) { shift = 1; return; } // shift down
    if (scancode == 0xAA || scancode == 0xB6) { shift = 0; return; } // shift up
    if (scancode & 0x80) return; // ignore key release


    if (scancode) terminal_on_key(scancode);
    outb(0x20, 0x20);
}
