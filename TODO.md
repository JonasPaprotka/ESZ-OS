# TODO
- Dynamic Heap Page allocation
- UX
    - arrow key cursor movement (left/right)
    - tab command completion for n>1 matches
    - consider: padding for text rendering
- Blinking cursor
- APIC
- ACIP
- Fix known bugs

# Future
- Filesystem FAT16, later FAT32
- Networking
    - translate linux drivers for the quemu intel network card

# Known Bugs
- first char at all is a space for some reasoning
- many new lines then "clear" -> crashes (page fault)
- cursor_backspace accesses cells[cursorAt_X - 1] BEFORE checking
- str_ends_with is completely broken:
- arrow DOWN through command history - go through history count - maybe out of bounds check
- terminal buffer off-by-one. checks lineInputLength > TERMINAL_BUFFER_SIZE instead of >=
- str_replace hardcoded stack array of 1024 max findings
