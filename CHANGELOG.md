# CHANGELOG

## v0.1.2 -

// TODO - this release planned changelog:
- Stability Update
    - Fixed known bugs
    - Fixed remaining compiler warnings

## v0.1.1 - 2025-06-13

- New functions in the string library
    - str_replace
    - str_move_left
    - str_trim_start
- Minor clean code / performance optimizations
- Config changes
    - Reduced init heap size from 32 MiB to 16 MiB
    - Switched default keyboard layout from DE to US
- Added a baic cursor renderer

## v0.1.0 - 2025-06-13

- Initial Release
    - bootable x86_64 kernel via Limine 12.3.3
    - framebuffer rendering
    - physical Memory Manager with bitmap
    - kernel heap allocator
    - keyboard input (US and DE layout)
    - terminal shell with commands: help, echo, clear, meminfo, uptime, reboot, sysinfo, history
    - PIT timer + sleep functions
    - IDT + fault handler
