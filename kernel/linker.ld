ENTRY(_start)
SECTIONS {
    . = 0x1000;
    
    .text : {
        kernel/kernel_entry.o(.text) 
        *(.text)
    }
    
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    
    _bss_start = .;
    .bss : { *(.bss) }
    _bss_end = .;
}