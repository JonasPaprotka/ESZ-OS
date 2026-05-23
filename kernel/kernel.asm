org 0x1000
bits 32

start:

    ;mov byte [0xB8000], 'z'
    ;mov byte [0xB8001], 0x07

    ;call clear

    jmp $


clear:
    mov edi, 2000 ; 2000 chars (80 chars per line 25 lines)    
    mov eax, 0xB8000 ; beginning adress
    call clear_loop
    ret

clear_loop:
    mov byte [eax], ' ' ; empty char
    inc eax
    mov byte [eax], 0x07 ; color grey - irrelevant but byte needed
    inc eax
    dec edi
    jnz clear_loop
    ret
