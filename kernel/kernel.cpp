#include "io.h"
#include "pic.h"
#include "idt.h"
#include "vga.h"
#include "terminal.h"

extern "C" void keyboard_isr();

extern "C" void kmain() {
    clear();
    terminal_init();

    pic_init();
    idt_init();
    idt_set_entry(33, (unsigned int) keyboard_isr);
    __asm__("sti");

    while(1); // loop to keep alive
}
