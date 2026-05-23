[bits 32]
[extern kmain]

global _start
_start:
    call kmain
    jmp $

[extern keyboard_handler]

global keyboard_isr
keyboard_isr:
    pusha
    call keyboard_handler
    popa
    iret
