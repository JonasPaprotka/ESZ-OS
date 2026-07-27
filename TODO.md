# TODO
- Make repo public
- Move Issues to GitHub-Issues
- Fix
    - known Bugs
- Tab autocomplete filename, and paths
- Filesystem FAT32
    - Write
- improve cd command
- String library improvements
    - str_to_int, str_to_hex
    - str_index_of
- Input handler for Questions (LIB)
    - Do you want to reboot now? (Y/N):
- Math library improvements
    - log2_floor(uint64_t n) -> uint64_t
    - int64_t pow_int(const int64_t base, const int64_t exp)
    - int64_t sqrt_int(const int64_t n)
- Memory
    - Dynamic Heap Page allocation
- Clean Code
    - Refactoring
    - Cleanup / Improvements
- UX
    - Tab command completion for n>1 matches
- Better Error handling and messages

# Future
- APIC
- ACPI
- Networking
    - translate linux drivers for the QEMU intel network card
- USB Drivers
    - 1.X
    - 3.X

# Known Bugs
- lineInputBuffer has no bounds check
- bit_* take int instead of uint64_t
- fat32_read derefs null when read_cluster_chain fails (fat32.cpp:237)
- free(nullptr)?
- memmove is aliased to forward-only memory_copy (libc.cpp)
- str_split has no bound on outSplits and never resets outSplitQty (string.cpp)
- str_ends_with is completely broken
- str_replace hardcoded stack array of 1024 max findings
- Multiline Terminal Commands are very broken
- map_pages has a open TODO
- to_int ignores base

# CONSIDER
- Unit Tests for lib and core functions
- Header file update to C++ specific version (.h -> .hpp)
- Padding for text rendering
