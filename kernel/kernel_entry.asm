bits 32

extern kmain
extern _bss_start
extern _bss_end
extern keyboard_handler

global _start
_start:
    call zero_bss
    call kmain
    jmp $


global keyboard_isr
keyboard_isr:
    pusha
    call keyboard_handler
    popa
    iret


zero_bss:
    mov edi, _bss_start
    call zero_bss_loop
    ret

zero_bss_loop:
    cmp edi, _bss_end
    je .done
    mov byte [edi], 0
    inc edi
    jmp zero_bss_loop
.done:
    ret
