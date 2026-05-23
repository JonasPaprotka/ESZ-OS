ENTRY(_start)
SECTIONS {
    . = 0x1000;
    .text : {
        kernel/entry.o(.text)
        *(.text)
    }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
}