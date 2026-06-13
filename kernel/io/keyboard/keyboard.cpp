#include "io.h"
#include "pic.h"
#include "print.h"
#include "terminal.h"

bool shift = false;
bool isExtendedScancode = false;

extern "C" void keyboard_handler() {
    __asm__ volatile("sti"); // allow timer interrupts while handling keyboard

    unsigned char scancode = inb(0x60);
    outb(0x20, 0x20);

    if (scancode == 0x2A || scancode == 0x36) { shift = 1; return; } // shift down
    if (scancode == 0xAA || scancode == 0xB6) { shift = 0; return; } // shift up
    if (scancode == 0xE0) { isExtendedScancode = true; return; }

    // ignore key release
    if (scancode & 0x80) {
        isExtendedScancode = false;
        return;
    }

    if (scancode) terminal_on_key(scancode);
}
