# ESZ-OS

# Install
```
brew install qemu nasm make
brew tap nativeos/i386-elf-toolchain
brew install i386-elf-binutils i386-elf-gcc
```

# Test
```
make clean && make run
```

# Known Bugs
- When command is longer then one line you cant backspace further than the "protected" area due to the check not knowing it is a continued line. Fix later - or rework with new display system in the future.