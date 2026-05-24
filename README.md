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
- First command after rebooting isnt known for some reason.