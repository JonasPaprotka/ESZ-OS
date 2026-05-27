#include "print.h"
#include "clear.h"
#include "terminal/terminal.h"
#include "helper/info_text.h"
#include "memory/memory.h"
#include "io/pic.h"
#include "io/idt.h"
#include "io/keyboard.h"
#include <stdint.h>


extern "C" void keyboard_isr();

extern "C" void kernel_main() {
    clearScreen();

    printInfoLine(InfoTextType::Loading, "Initializing Memory Info...");
    memory_info_init();
    
    printInfoLine(InfoTextType::Loading, "Initializing Terminal...");
    terminal_init();
    
    pic_init();

    idt_init();
    idt_set_entry(33, (uint64_t) keyboard_isr); 
    
    __asm__ volatile("sti"); 

    while(1) {
        __asm__ volatile("hlt");
    }
}
