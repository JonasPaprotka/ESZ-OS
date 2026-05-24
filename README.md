# ESZ-OS

W.I.P.
by Jonas Paprotka

# Install
```powershell
brew install qemu nasm make
brew tap nativeos/i386-elf-toolchain
brew install i386-elf-binutils i386-elf-gcc
```

# Test
```powershell
make clean && make run
```

# Known Bugs
- When command is longer then one line you cant backspace further than the "protected" area due to the check not knowing it is a continued line. Fix later - or rework with new display system in the future.

# Further TODO
- Kernel panic system - handle e.g. stuff like division by 0 errors from the CPU
- Implement system timer and sleep() function or something simmilar - for multitasking for later stuff
- Hex to string, int to string and further TODO labeld functions in code
- Ram manager - get info from bios - then make a ram map map usage and stuff (kill me)
- BIS+32bit -> 64bit mode & UEFI update
