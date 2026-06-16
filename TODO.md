# TODO
- Fix known bugs
- UX
    - tab command completion for n>1 matches
    - consider: padding for text rendering
- Dynamic Heap Page allocation
- Refactoring & Code cleanup/improvements

# Future
- Filesystem FAT16, later FAT32
- APIC
- ACIP
- Networking
    - translate linux drivers for the quemu intel network card
- Unit Tests für lib and core functions
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
