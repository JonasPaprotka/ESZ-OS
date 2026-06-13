# ESZ-OS

v0.1.0

A hobby x86_64 kernel written in C++ and Assembly

## Disclaimer

- Im new to low level and C++ - this is not ideal for this project if you can imagine
- For some code parts AI was used as a help - All of them are marked as AI Supported via code comment (i will revisit every single one of them again and rewrite it - probably make it worse but at the end code should 100% be human generated - even if it sucks)

## Development / Testing Requirements

- macOS (tested), Linux should work
- QEMU for testing

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

You can also simulate more RAM with the given presets
 - run -> 512 MB
 - run-1 -> 1024 MB
 - run-2 -> 2048 MB
 - run-4 -> 4096 MB
 - run-8 -> 8192 MB
 - run-16 -> 16384 MB
 - run-32 -> 32768 MB
 - run-64 -> 65536 MB
