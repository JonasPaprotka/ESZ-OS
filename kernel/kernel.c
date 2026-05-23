#include "io.h"
#include "pic.h"
#include "idt.h"
#include "vga.h"

void printHeader() {
    printf("--------------------- ", DarkGray, 0, 0);
    printf("[INFO]: ", Yellow, 1, 0);
    printf("Kernel Loaded", White, 1, 8);
    printf("ESZ-OS", White, 2, 0);
    printf("by Jonas Paprotka", White, 3, 0);
    printf("--------------------- ", DarkGray, 4, 0);
}

extern void keyboard_isr();

void kmain() {
    clear();
    printHeader();

    pic_init();
    idt_init();
    idt_set_entry(33, (unsigned int) keyboard_isr);
    __asm__("sti");

    while(1); // loop to keep alive
}
