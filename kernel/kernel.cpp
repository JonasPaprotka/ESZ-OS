#include <stdint.h>
#include "print.h"
#include "clear.h"
#include "terminal.h"
#include "info_text.h"
#include "memory.h"
#include "pic.h"
#include "pit.h"
#include "idt.h"
#include "io.h"
#include "keyboard.h"
#include "screenBuffer.h"
#include "storage.h"

void halt() {
    while(1) {
        __asm__ volatile("hlt");
    }
}

struct Registers {
    //pushed common registers
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rdx, rcx, rbx, rax;

    uint64_t interrupt_number, error_code; //pushed error number
    uint64_t rip, cs, rflags, rsp, ss; //pushed by cpu before stub runs
} __attribute__((packed));

extern "C" void isr_stub_0();
extern "C" void isr_stub_1();
extern "C" void isr_stub_2();
extern "C" void isr_stub_3();
extern "C" void isr_stub_4();
extern "C" void isr_stub_5();
extern "C" void isr_stub_6();
extern "C" void isr_stub_7();
extern "C" void isr_stub_8();
extern "C" void isr_stub_9();
extern "C" void isr_stub_10();
extern "C" void isr_stub_11();
extern "C" void isr_stub_12();
extern "C" void isr_stub_13();
extern "C" void isr_stub_14();
extern "C" void isr_stub_15();
extern "C" void isr_stub_16();
extern "C" void isr_stub_17();
extern "C" void isr_stub_18();
extern "C" void isr_stub_19();
extern "C" void isr_stub_20();
extern "C" void isr_stub_21();
extern "C" void isr_stub_22();
extern "C" void isr_stub_23();
extern "C" void isr_stub_24();
extern "C" void isr_stub_25();
extern "C" void isr_stub_26();
extern "C" void isr_stub_27();
extern "C" void isr_stub_28();
extern "C" void isr_stub_29();
extern "C" void isr_stub_30();
extern "C" void isr_stub_31();

extern "C" void timer_isr(); // 32
extern "C" void keyboard_isr(); //33

extern "C" void fault_handler(Registers* regs) {
    newline();
    print_separator();

    switch (regs->interrupt_number) {
        case 0:
            printInfoLine(InfoTextType::KernelPanic, "DIVIDE BY ZERO FAULT");
            break;
        case 1:
            printInfoLine(InfoTextType::KernelPanic, "DEBUG FAULT / TRAP");
            break;
        case 2:
            printInfoLine(InfoTextType::KernelPanic, "NON-MASKABLE INTERRUPT");
            break;
        case 3:
            printInfoLine(InfoTextType::KernelPanic, "BREAKPOINT TRAP");
            break;
        case 4:
            printInfoLine(InfoTextType::KernelPanic, "OVERFLOW TRAP");
            break;
        case 5:
            printInfoLine(InfoTextType::KernelPanic, "BOUND RANGE EXCEEDED FAULT");
            break;
        case 6:
            printInfoLine(InfoTextType::KernelPanic, "INVALID OPCODE FAULT");
            break;
        case 7:
            printInfoLine(InfoTextType::KernelPanic, "DEVICE NOT AVAILABLE FAULT");
            break;
        case 8:
            printInfoLine(InfoTextType::KernelPanic, "DOUBLE FAULT ABORT");
            break;
        case 9:
            printInfoLine(InfoTextType::KernelPanic, "COPROCESSOR SEGMENT OVERRUN");
            break;
        case 10:
            printInfoLine(InfoTextType::KernelPanic, "INVALID TSS FAULT");
            break;
        case 11:
            printInfoLine(InfoTextType::KernelPanic, "SEGMENT NOT PRESENT FAULT");
            break;
        case 12:
            printInfoLine(InfoTextType::KernelPanic, "STACK-SEGMENT FAULT");
            break;
        case 13:
            printInfoLine(InfoTextType::KernelPanic, "GENERAL PROTECTION FAULT");
            break;
        case 14: {
            uint64_t cr2;
            __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));

            printInfoLine(InfoTextType::KernelPanic, "PAGE FAULT");

            print_inline("[REASON]: ", Color::LightRed);
            if (!(regs->error_code & 1)) print_inline("not-present ");
            else                         print_inline("protection-violation ");
            if (regs->error_code & 2)    print_inline("/ write ");
            else                         print_inline("/ read ");
            if (regs->error_code & 4)    print_inline("/ user ");
            else                         print_inline("/ kernel ");
            if (regs->error_code & 8)    print_inline("/ reserved-bit-violation ");
            if (regs->error_code & 16)   print_inline("/ instruction-fetch ");
            newline();

            const char* hexString = to_string(cr2, 16);
            printInfoLine(InfoTextType::PanicInfo, String("FAULT ADDR: ", hexString));
            free(hexString);

            break;
        }
        case 15:
            printInfoLine(InfoTextType::KernelPanic, "RESERVED");
            break;
        case 16:
            printInfoLine(InfoTextType::KernelPanic, "X87 FLOATING-POINT EXCEPTION");
            break;
        case 17:
            printInfoLine(InfoTextType::KernelPanic, "ALIGNMENT CHECK FAULT");
            break;
        case 18:
            printInfoLine(InfoTextType::KernelPanic, "MACHINE CHECK ABORT");
            break;
        case 19:
            printInfoLine(InfoTextType::KernelPanic, "SIMD FLOATING-POINT EXCEPTION");
            break;
        case 20:
            printInfoLine(InfoTextType::KernelPanic, "VIRTUALIZATION EXCEPTION");
            break;
        case 21:
            printInfoLine(InfoTextType::KernelPanic, "CONTROL PROTECTION EXCEPTION");
            break;
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
            printInfoLine(InfoTextType::KernelPanic, "RESERVED");
            break;
        default:
            printInfoLine(InfoTextType::KernelPanic, "UNKNOWN FAULT");
            break;
    }

    char* hexString = to_string(regs->rip, 16);
    printInfoLine(InfoTextType::PanicInfo, String("RIP: ", hexString));
    free(hexString);
    hexString = to_string(regs->rsp, 16);
    printInfoLine(InfoTextType::PanicInfo, String("RSP: ", hexString));
    free(hexString);
    hexString = to_string(regs->cs, 16);
    printInfoLine(InfoTextType::PanicInfo, String("CS: ", hexString));
    free(hexString);
    hexString = to_string(regs->error_code, 16);
    printInfoLine(InfoTextType::PanicInfo, String("ERROR: ", hexString));
    free(hexString);

    print_separator();
    halt();
}

void populate_idt_entries() {
    idt_set_entry(0, (uint64_t) isr_stub_0);
    idt_set_entry(1, (uint64_t) isr_stub_1);
    idt_set_entry(2, (uint64_t) isr_stub_2);
    idt_set_entry(3, (uint64_t) isr_stub_3);
    idt_set_entry(4, (uint64_t) isr_stub_4);
    idt_set_entry(5, (uint64_t) isr_stub_5);
    idt_set_entry(6, (uint64_t) isr_stub_6);
    idt_set_entry(7, (uint64_t) isr_stub_7);
    idt_set_entry(8, (uint64_t) isr_stub_8);
    idt_set_entry(9, (uint64_t) isr_stub_9);
    idt_set_entry(10, (uint64_t) isr_stub_10);
    idt_set_entry(11, (uint64_t) isr_stub_11);
    idt_set_entry(12, (uint64_t) isr_stub_12);
    idt_set_entry(13, (uint64_t) isr_stub_13);
    idt_set_entry(14, (uint64_t) isr_stub_14);
    idt_set_entry(15, (uint64_t) isr_stub_15);
    idt_set_entry(16, (uint64_t) isr_stub_16);
    idt_set_entry(17, (uint64_t) isr_stub_17);
    idt_set_entry(18, (uint64_t) isr_stub_18);
    idt_set_entry(19, (uint64_t) isr_stub_19);
    idt_set_entry(20, (uint64_t) isr_stub_20);
    idt_set_entry(21, (uint64_t) isr_stub_21);
    idt_set_entry(22, (uint64_t) isr_stub_22);
    idt_set_entry(23, (uint64_t) isr_stub_23);
    idt_set_entry(24, (uint64_t) isr_stub_24);
    idt_set_entry(25, (uint64_t) isr_stub_25);
    idt_set_entry(26, (uint64_t) isr_stub_26);
    idt_set_entry(27, (uint64_t) isr_stub_27);
    idt_set_entry(28, (uint64_t) isr_stub_28);
    idt_set_entry(29, (uint64_t) isr_stub_29);
    idt_set_entry(30, (uint64_t) isr_stub_30);
    idt_set_entry(31, (uint64_t) isr_stub_31);

    idt_set_entry(32, (uint64_t) timer_isr);
    idt_set_entry(33, (uint64_t) keyboard_isr);
}

// --- MAIN KERNEL ---
extern "C" void kernel_main() {
    clearScreen();
    init_print();

    idt_init();
    populate_idt_entries();

    memory_info_init();

    pit_init();
    pic_init();

    // flush pending scancodes
    while (inb(0x64) & 1) inb(0x60);

    init_screen_buffer();

    init_storage();

    terminal_init();

    __asm__ volatile("sti");
    halt();
}
