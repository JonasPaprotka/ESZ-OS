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
- Unit Tests für lib and core functions

# Known Bugs
- str_ends_with is completely broken
- str_replace hardcoded stack array of 1024 max findings
- Multiline Terminal Commands are very broken
- After scrolling - fake chars can appear on cursor (from line above)
- "Test mem_ 123" _ signaling the cursor -> on tab nothing happens 
