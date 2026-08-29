# ESZ-OS

A hobby x86_64 kernel written in C++ and Assembly

[![Build](https://img.shields.io/github/actions/workflow/status/JonasPaprotka/ESZ-OS/build.yml?branch=main&label=build)](https://github.com/JonasPaprotka/ESZ-OS/actions/workflows/build.yml)
[![Tests](https://img.shields.io/github/actions/workflow/status/JonasPaprotka/ESZ-OS/tests.yml?branch=main&label=tests)](https://github.com/JonasPaprotka/ESZ-OS/actions/workflows/tests.yml)
[![Release](https://img.shields.io/github/v/release/JonasPaprotka/ESZ-OS)](https://github.com/JonasPaprotka/ESZ-OS/releases/latest)
[![License](https://img.shields.io/github/license/JonasPaprotka/ESZ-OS)](LICENSE)

![ESZ-OS](docs/images/hero.gif)

## What works right now

- Boots on QEMU and real hardware
- Framebuffer text rendering
- Physical memory manager and a kernel heap allocator
- PS/2 keyboard (US and DE layout)
- PCI bus enumeration
- AHCI/SATA driver (IDENTIFY, READ DMA EXT, FLUSH CACHE EXT)
- MBR partition parsing
- FAT32 (currently read only)
- Shell with command history, cursor editing and tab completion (completion not yet for filesystem)
    - help, echo, clear, sysinfo, meminfo, uptime, history, reboot, pciinfo (-c), driveinfo, dumpsector, ls, cd, pwd, read

## Disclaimer

- I'm new to low level and C++ - this is not ideal for this project (if you can imagine)
- The main usage of AI was for teaching, not coding. If something was written by AI e.g. the Makefile, it is labeled with a code comment

# "Install" the OS

Currently you can run the OS on real hardware but not fully install it yet. SATA is supported, yes but FAT32 only supports read in the current version. This might change soon.
Every version gets built and published as a **.iso** file you can download and use your iso-USB burner tool of your choice e.g. RUFUS or balenaEtcher - depends on your current running OS.
Honestly the current state of the OS is very useless, which will remain in this state for a long period of time.

# Development

- macOS works (tested)
- Linux should work
- Windows should work

**Cpp-Version: 20**

## Configuration

Configuration of this OS happens inside the `config.h` file. There you can set your font size, keyboard layout, the cursor blink interval and much more.

## DEV-Sandbox Usage

### macOS

#### Install

```zsh
brew install x86_64-elf-gcc x86_64-elf-binutils nasm xorriso mtools qemu
mkdir -p limine kernel/vendor
curl -fL -o limine.tar.xz https://github.com/limine-bootloader/limine/releases/download/v12.6.1/limine-binary.tar.xz
curl -fL -o kernel/vendor/limine.h https://raw.githubusercontent.com/limine-bootloader/limine-protocol/trunk/include/limine.h
curl -fL -o kernel/vendor/stb_truetype.h https://raw.githubusercontent.com/nothings/stb/master/stb_truetype.h
tar -xf limine.tar.xz -C limine --strip-components=1
rm limine.tar.xz
make -C limine
make disk
make clean && make
```

#### Update

```zsh
brew upgrade x86_64-elf-gcc x86_64-elf-binutils nasm xorriso mtools qemu
rm -rf limine
mkdir -p limine kernel/vendor
curl -fL -o limine.tar.xz https://github.com/limine-bootloader/limine/releases/download/v12.6.1/limine-binary.tar.xz
tar -xf limine.tar.xz -C limine --strip-components=1
rm limine.tar.xz
make -C limine
curl -fL -o kernel/vendor/limine.h https://raw.githubusercontent.com/limine-bootloader/limine-protocol/trunk/include/limine.h
curl -fL -o kernel/vendor/stb_truetype.h https://raw.githubusercontent.com/nothings/stb/master/stb_truetype.h
make disk
make clean
```

#### Test

```zsh
make clean && make run
```

## Makefile

Targets
 - run -> boot the ISO in QEMU
 - run-ide -> same but with an IDE disk
 - run-debug -> logs interrupts and CPU resets to bin/qemu.log
 - debug -> starts halted with a gdb stub on :1234
 - disk -> rebuild the FAT32 disk image
 - disk-files -> copy rootfs/ into the existing image
 - clean / clean-disk

You can override the QEMU settings on the command line

```zsh
make run MEM=8G
make run SMP=4 # CPU cores
make disk DISK_SIZE=32G
```

The Makefile was heavily edited with AI due to my lack of interest in learning Makefile - I will revisit it again and make it better... or worse, but human.

## Third Party

### Limine Bootloader

ESZ-OS boots via [Limine](https://github.com/limine-bootloader/limine), which is licensed under BSD-2-Clause. The published **.iso** ships Limine binaries.

```
Copyright (C) 2019-2026 Mintsuki and contributors.
```

## Versioning

### New Version

To release a new version of ESZ-OS you need to be on the pulled commit on the main branch and tag it. The pipeline will automatically build the release, add the changelog and publish it alongside the .iso file.
Only the maintainers of the repository are able to release a new version.

```zsh
git tag <version>
git push origin tag <version>
```

## Tests

After switching your C++ configuration to the test version, you can run `make -C tests` to run the tests manually.
