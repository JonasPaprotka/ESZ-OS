#include "print.h"
#include "clear.h"
#include "terminal/terminal.h"
#include "helper/info_text.h"
#include "memory/memory.h"
#include "io/pic.h"
#include "io/idt.h"
#include "io/keyboard.h"
#include <stdint.h>


void halt() {
    while(1) {
        __asm__ volatile("hlt");
    }
}

extern "C" void keyboard_isr();


extern "C" void panic() {
    printInfoLine(InfoTextType::KernelPanic, "CPU FAULT");
    halt();
}

void populate_idt_entries() {
    //TODO handle correctly
    for (int i = 0; i <= 31; i++) {
        idt_set_entry(i, (uint64_t) panic); 
    }

    idt_set_entry(33, (uint64_t) keyboard_isr); 
}

extern "C" void kernel_main() {
    idt_init();

    clearScreen();
    init_print();

    populate_idt_entries();


    printInfoLine(InfoTextType::Loading, "Initializing Memory Info...");
    memory_info_init();
    
    printInfoLine(InfoTextType::Loading, "Initializing Terminal...");
    terminal_init();
    
    pic_init();

    __asm__ volatile("sti"); 
    halt();
}
