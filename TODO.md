# TODO
- String library improvements
    - str_to_int, str_to_hex
    - str_index_of
    - str_split
- Fix
    - Known Bugs
- Memory
    - Dynamic Heap Page allocation
- Clean Code
    - Refactoring
    - Cleanup / Improvements
- UX
    - Tab command completion for n>1 matches

# Future
- Unit Tests für lib and core functions
- Filesystem FAT16, later FAT32
- APIC
- ACIP
- Networking
    - translate linux drivers for the quemu intel network card
- USB Drivers
    - 1.X
    - 3.X

# Known Bugs
- str_ends_with is completely broken
- str_replace hardcoded stack array of 1024 max findings
- Multiline Terminal Commands are very broken
- After scrolling - fake chars can appear on cursor (from line above)

# CONSIDER
- Header file update to C++ specific verion (.h -> .hpp)
- Padding for text rendering
