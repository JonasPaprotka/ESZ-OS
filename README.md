# ESZ-OS

# Install
```zsh
brew install x86_64-elf-gcc x86_64-elf-binutils nasm xorriso mtools qemu
curl -L -o limine.tar.xz https://github.com/limine-bootloader/limine/releases/download/v12.3.3/limine-binary.tar.xz
tar -xf limine.tar.xz -C limine --strip-components=1
rm limine.tar.xz
make -C limine
```

get limine.h
```zsh
curl -L -o kernel/types/limine.h https://raw.githubusercontent.com/limine-bootloader/limine-protocol/trunk/include/limine.h
```

# Update
```zsh
rm -rf limine
mkdir -p limine
curl -L -o limine.tar.xz https://github.com/limine-bootloader/limine/releases/download/v12.3.3/limine-binary.tar.xz
tar -xf limine.tar.xz -C limine --strip-components=1
rm limine.tar.xz
make -C limine
curl -L -o kernel/types/limine.h https://raw.githubusercontent.com/limine-bootloader/limine-protocol/trunk/include/limine.h
make clean
```

# Test
```zsh
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
