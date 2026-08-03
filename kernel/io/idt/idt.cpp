// File Updated with AI / Assisted by AI

#include "idt.h"

struct IDTEntry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  flags;
    uint16_t base_mid; // middle 16 bits of handler address
    uint32_t base_high; // higher 32 bits of handler address
    uint32_t reserved;
} __attribute__((packed));

struct IDTDescriptor {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

struct IDTEntry idt[256];
struct IDTDescriptor idt_desc;

void idt_set_entry(int n, uint64_t handler) {
    idt[n].base_low  = handler & 0xFFFF;
    idt[n].base_mid  = (handler >> 16) & 0xFFFF;
    idt[n].base_high = (handler >> 32) & 0xFFFFFFFF;
    idt[n].selector = 0x28; // Limine default for 64 bit
    idt[n].ist = 0;
    idt[n].flags = 0x8E;
    idt[n].reserved = 0;
}

void idt_init() {
    idt_desc.limit = sizeof(idt) - 1;
    idt_desc.base  = (uint64_t)&idt;

    for (int i = 0; i < 256; i++) {
        idt[i].flags = 0;
    }

    // load the IDT into the CPU register
    __asm__ volatile("lidt %0" : : "m"(idt_desc));
}
