# TODO
- Partition Table Format
    - MBR (first due to it being simpler)
    - GPT (later)
- Filesystem FAT32
- String library improvements
    - str_to_int, str_to_hex
    - str_index_of
    - str_split
- Math libary improvements
    - log2_floor(uint64_t n) -> uint64_t
    - int64_t pow_int(const int64_t base, const int64_t exp)
    - int64_t sqrt_int(const int64_t n)
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
- On real hardware the primary SATA detection fails sometimes for some reason

# CONSIDER
- Unit Tests für lib and core functions
- Header file update to C++ specific verion (.h -> .hpp)
- Padding for text rendering
