/* ESZ-OS Linker Script - 64-bit Higher Half (Limine) */

OUTPUT_FORMAT(elf64-x86-64)
ENTRY(kmain)

PHDRS {
    limine_requests PT_LOAD;
    text            PT_LOAD;
    rodata          PT_LOAD;
    data            PT_LOAD;
}

SECTIONS {
    /* Kernel ins Higher Half (Konvention für 64-bit Kernel) */
    . = 0xffffffff80000000;

    /* Limine Requests MÜSSEN ganz am Anfang stehen,
       damit der Bootloader sie scannt */
    .limine_requests : {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } :limine_requests

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .text : {
        *(.text .text.*)
    } :text

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .data : {
        *(.data .data.*)
    } :data

    .bss : {
        *(COMMON)
        *(.bss .bss.*)
    } :data

    /DISCARD/ : {
        *(.eh_frame*)
        *(.note .note.*)
    }
}