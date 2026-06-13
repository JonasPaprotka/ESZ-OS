# CHANGELOG

## v0.1.2

- Stability Update
    - Fixed known bugs
        - Clear command crashed the Kernel sometimes
        - Fixed incorrect handling on Command History next (ARROW DOWN)
        - Fixed error handling for max command char - off by one error
        - Fixed backspace out of bounds possibility when cursor at X 0
        - Fixed the error which made the first char not being rendered correctly
        - Fixed screen buffer memory leak and not being cleared correctly
    - Fixed remaining compiler warnings

## v0.1.1

- New functions in the string library
    - str_replace
    - str_move_left
    - str_trim_start
- Minor clean code / performance optimizations
- Config changes
    - Reduced init heap size from 32 MiB to 16 MiB
    - Switched default keyboard layout from DE to US
- Added a baic cursor renderer

## v0.1.0

- Initial Release
    - bootable x86_64 kernel via Limine 12.3.3
    - framebuffer rendering
    - physical Memory Manager with bitmap
    - kernel heap allocator
    - keyboard input (US and DE layout)
    - terminal shell with commands: help, echo, clear, meminfo, uptime, reboot, sysinfo, history
    - PIT timer + sleep functions
    - IDT + fault handler
