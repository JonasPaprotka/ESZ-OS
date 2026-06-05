# ESZ-OS

# Install
bios 32 bit edition (outdated)
```powershell
brew install qemu nasm make
brew tap nativeos/i386-elf-toolchain
brew install i386-elf-binutils i386-elf-gcc
```

new uefi 64 bit edition
```powershell
brew install x86_64-elf-gcc x86_64-elf-binutils nasm xorriso mtools qemu
git clone https://github.com/limine-bootloader/limine.git --branch=v9.x-binary --depth=1 limine
make -C limine
```

# Test
```powershell
make clean && make run
```

# TODO
- Dynamic Heap Page allocation
- UX
    - arrow key cursor movement
    - tab: command completion
    - Padding for text rendering
- Implement system timer and sleep()
- APIC
- ACIP
- implement `str_replace` string.cpp

# Future TODOs
- Filesystem FAT16, later FAT32
- Networking (hehe)
    - translate linux drivers for the quemu intel network card
    - die
