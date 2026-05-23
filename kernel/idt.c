// AI

#include "idt.h"

struct IDTEntry {
    unsigned short base_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short base_high;
} __attribute__((packed));

struct IDTDescriptor {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct IDTEntry idt[256];
struct IDTDescriptor idt_desc;

void idt_set_entry(int n, unsigned int handler) {
    idt[n].base_low  = handler & 0xFFFF;
    idt[n].base_high = (handler >> 16) & 0xFFFF;
    idt[n].selector  = 0x08;
    idt[n].zero      = 0;
    idt[n].flags     = 0x8E;
}

void idt_init() {
    idt_desc.limit = sizeof(idt) - 1;
    idt_desc.base  = (unsigned int) &idt;

    // null table
    for (int i = 0; i < 256; i++) {
        idt_set_entry(i, 0);
    }

    __asm__("lidt %0" : : "m"(idt_desc));
}
