#ifndef IDT_FAULTS_H
#define IDT_FAULTS_H

#include "integer.h"

struct Registers {
    //pushed common registers
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rdx, rcx, rbx, rax;

    uint64_t interrupt_number, error_code; //pushed error number
    uint64_t rip, cs, rflags, rsp, ss; //pushed by cpu before stub runs
} __attribute__((packed));

void populate_idt_entries();

#endif // IDT_FAULTS_H
