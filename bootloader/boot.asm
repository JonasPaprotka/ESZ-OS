org 0x7c00 ; bios requirement
bits 16

start:
    ; cleanup
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov sp, 0x7c00 ; correct placement - make that stack dosnt overwrite bios data

    mov si, boot_msg
    call print

    mov si, loading_kernel_msg
    call print
    call load_kernel_from_disk

    mov si, configuring_GDT_msg
    call print
    call configure_GDT

    mov si, getting_memory_map_msg
    call print
    call get_memory_map

    mov si, activate_protected_mode_msg
    call print
    call activate_protected_mode ; activates kernel in process

    ; jmp halt


boot_msg: db "Booting ESZ-OS", 0x0d, 0x0a, 0
loading_kernel_msg: db "Loading kernel from disk", 0x0d, 0x0a, 0
configuring_GDT_msg: db "Configuring GDT", 0x0d, 0x0a, 0
activate_protected_mode_msg: db "Activating protected mode", 0x0d, 0x0a, 0
getting_memory_map_msg: db "Getting memory map", 0x0d, 0x0a, 0


print:
    lodsb
    or al, al
    jz .done

    mov ah, 0x0e
    mov bh, 0x00
    mov bl, 0x07
    int 0x10

    jmp print
.done:
    ret ; back to caller


halt:
    jmp $ ; jump back -> inf loop


load_kernel_from_disk:
    mov ah, 2  ; read secotrs
    mov al, 40 ; sector amount (40 = 20KB)
    mov ch, 0 ; cylinder amount
    mov cl, 2 ; kernel on sector (sector 1 bootloader, 2 kernel)
    mov dh, 0 ; head (which side on plate i would assume)
    ; dl - drive number filled by os - hopefully
    ; es - start address (set to 0 above)
    mov bx, 0x1000 ; end adress

    int 0x13
    ret

configure_GDT:
    lgdt [gdt_descriptor]
    ret

activate_protected_mode:
    cli
    mov eax, cr0
    or eax, 1 ; set bit 0 to 1
    mov cr0, eax
    jmp 0x08:protected_mode_entry

gdt_start:
    ; null descriptor
    dd 0x00000000
    dd 0x00000000

    ; code segment
    dw 0xFFFF, 0x0000
    db 0x00, 0x9A, 0xCF, 0x00

    ; data segment
    dw 0xFFFF, 0x0000
    db 0x00, 0x92, 0xCF, 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start


get_memory_map:
    xor ax, ax
    mov es, ax
    mov ebx, 0
    mov di, 0x8004 ; entry region
    mov bp, 0 ; counter
    call get_memory_map_loop
    ret

get_memory_map_loop:
    mov eax, 0xe820
    mov edx, 0x534D4150 ; "SMAP"
    mov ecx, 24

    int 0x15
    
    jc .done

    inc bp ; counter ++
    add di, 24

    cmp ebx, 0
    je .done

    jmp get_memory_map_loop
.done:
    mov [0x8000], bp ; 4 byte before entry array (0x8004)
    ret


[bits 32]
protected_mode_entry:
    mov ax, 0x10 ; data segment selector
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x90000 ; new stack for 32-bit
    jmp 0x1000 ; goto kernel

; padding -> 512 bytes
times 510 - ($ - $$) db 0x00
db 0x55
db 0xaa
