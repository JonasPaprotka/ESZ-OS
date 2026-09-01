#include <stdint.h>
#include "print.h"
#include "clear.h"
#include "screenBuffer.h"
#include "line_editor.h"
#include "print_helper.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "io.h"
#include "pci.h"
#include "ahci.h"
#include "memory_init.h"
#include "heap.h"
#include "string.h"
#include "filesystem.h"
#include "idt_faults.h"

static void init_rendering_early() {
    clearScreen();
    init_print();
}

static void init_interrupts() {
    idt_init();
    populate_idt_entries();
    pit_init();
    pic_init();
    while (inb(0x64) & 1) inb(0x60);
}

static void init_memory() {
    printLoadingStart("Memory");
    printLoadingStatus(memory_init());
}

static void init_rendering_full() {
    printLoadingStart("Screen Buffer");
    printLoadingStatus(init_screen_buffer());
}

static void init_pci_layer() {
    printLoadingStart("PCI Bus Enumeration");
    printLoadingStatus(init_pci());
}

static void init_drive_layer() {
    printLoadingStart("AHCI Controller");
    printLoadingStatus(init_ahci());
}

static void init_filesystem_layer() {
    printLoadingStart("Filesystem");
    printLoadingStatus(init_filesystem());
}

static void init_shell() {
    terminal_init();
}

extern "C" void kernel_main() {
    // STAGE 1
    init_interrupts();
    init_rendering_early();

    // STAGE 2
    init_memory();

    // STAGE 3
    init_rendering_full();
    init_pci_layer();
    init_drive_layer();
    init_filesystem_layer();

    // STAGE 4
    init_shell();

    __asm__ volatile("sti");
    halt();
}
