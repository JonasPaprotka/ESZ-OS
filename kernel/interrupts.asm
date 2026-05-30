global isr_stub_0
global isr_stub_1
global isr_stub_2
global isr_stub_3
global isr_stub_4
global isr_stub_5
global isr_stub_6
global isr_stub_7
global isr_stub_8
global isr_stub_9
global isr_stub_10
global isr_stub_11
global isr_stub_12
global isr_stub_13
global isr_stub_14
global isr_stub_15
global isr_stub_16
global isr_stub_17
global isr_stub_18
global isr_stub_19
global isr_stub_20
global isr_stub_21
global isr_stub_22
global isr_stub_23
global isr_stub_24
global isr_stub_25
global isr_stub_26
global isr_stub_27
global isr_stub_28
global isr_stub_29
global isr_stub_30
global isr_stub_31

extern fault_handler

isr_stub_0:
    push 0 ; error code
    push 0 ; interrupt number (divide by zero)
    jmp isr_common

isr_stub_1:
    push 0 ; error code
    push 1 ; interrupt number (debug)
    jmp isr_common

isr_stub_2:
    push 0 ; error code
    push 2 ; interrupt number (non maskable interupt)
    jmp isr_common

isr_stub_3:
    push 0 ; error code
    push 3 ; interrupt number (breakpoint)
    jmp isr_common

isr_stub_4:
    push 0 ; error code
    push 4 ; interrupt number (overflow)
    jmp isr_common

isr_stub_5:
    push 0 ; error code
    push 5 ; interrupt number (bound range exceeded)
    jmp isr_common

isr_stub_6:
    push 0 ; error code
    push 6 ; interrupt number (invalid opcode)
    jmp isr_common

isr_stub_7:
    push 0 ; error code
    push 7 ; interrupt number (device not available)
    jmp isr_common

isr_stub_8:
    push 8 ; interrupt number (double fault)
    jmp isr_common

isr_stub_9:
    push 0 ; error code
    push 9 ; interrupt number (compocesser segment overrun)
    jmp isr_common

isr_stub_10:
    push 10 ; interrupt number (invalid tss)
    jmp isr_common

isr_stub_11:
    push 11 ; interrupt number (segment not present)
    jmp isr_common

isr_stub_12:
    push 12 ; interrupt number (stack segment fault)
    jmp isr_common

isr_stub_13:
    push 13 ; interrupt number (general protection fault)
    jmp isr_common

isr_stub_14:
    push 14 ; interrupt number (page fault)
    jmp isr_common

isr_stub_15:
    push 0 ; error code
    push 15 ; interrupt number (reserved)
    jmp isr_common

isr_stub_16:
    push 0 ; error code
    push 16 ; interrupt number (x87 floating point exception)
    jmp isr_common

isr_stub_17:
    push 17 ; interrupt number (allignment check)
    jmp isr_common

isr_stub_18:
    push 0 ; error code
    push 18 ; interrupt number (machine check)
    jmp isr_common

isr_stub_19:
    push 0 ; error code
    push 19 ; interrupt number (simd floating point exception)
    jmp isr_common

isr_stub_20:
    push 0 ; error code
    push 20 ; interrupt number (virtualization exception)
    jmp isr_common

isr_stub_21:
    push 21 ; interrupt number (control protection exception)
    jmp isr_common

isr_stub_22:
    push 0 ; error code
    push 22 ; interrupt number (reserved)
    jmp isr_common

isr_stub_23:
    push 0 ; error code
    push 23 ; interrupt number (reserved)
    jmp isr_common

isr_stub_24:
    push 0 ; error code
    push 24 ; interrupt number (reserved)
    jmp isr_common

isr_stub_25:
    push 0 ; error code
    push 25 ; interrupt number (reserved)
    jmp isr_common

isr_stub_26:
    push 0 ; error code
    push 26 ; interrupt number (reserved)
    jmp isr_common

isr_stub_27:
    push 0 ; error code
    push 27 ; interrupt number (reserved)
    jmp isr_common

isr_stub_28:
    push 0 ; error code
    push 28 ; interrupt number (reserved)
    jmp isr_common

isr_stub_29:
    push 0 ; error code
    push 29 ; interrupt number (reserved)
    jmp isr_common

isr_stub_30:
    push 0 ; error code
    push 30 ; interrupt number (reserved)
    jmp isr_common

isr_stub_31:
    push 0 ; error code
    push 31 ; interrupt number (reserved)
    jmp isr_common


isr_common:
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    ; c++ handoff
    mov rdi, rsp 
    call fault_handler
