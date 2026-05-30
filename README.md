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
- Implement Heap Allocator - no more Page Allocation for everything (kmalloc, kfree)
- Rewrite line rendering - actually store all line
  - Allow scroll up/down
- UX
    - arrow key cursor movement
    - tab: command completion
- Implement system timer and sleep()
- Shutdown, Reboot, etc via APIC
- Hex to string (and fix TODOs in string.cpp)

# Future TODOs
- Filesystem FAT16, later FAT32
- Networking (hehe)
    - translate linux drivers for the quemu intel network card
    - die
