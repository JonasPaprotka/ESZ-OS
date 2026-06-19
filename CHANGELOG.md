# CHANGELOG

## v0.1.9

-

## v0.1.8

- Math libary improvements
    - align_up(uint64_t addr, uint64_t align) -> uint64_t
    - align_down(uint64_t addr, uint64_t align) -> uint64_t
    - is_aligned(uint64_t addr, uint64_t align) -> bool
- PCI Class Name Resolvment for pciinfo command

## v0.1.7

- PCI
    - Parse PCI Devices and BARs
    - Added new command to dump PCI Device Informations "pciinfo" which supports "-c" arg to compress output
- Added MIT License File
- Seperator length is now printed 1/3 of max line chars
- Fix memory leak on kernel panic info dump
- Developer Infos
    - Fixed Typos
    - Improved and expanded io library
        - Fixed outb and inb not using correct stdints
        - Added following new functions
            - void outw(const uint16_t port, const uint16_t value);
            - uint16_t inw(const uint16_t port);
            - void outl(const uint16_t port, const uint32_t value);
            - uint32_t inl(const uint16_t port);
    - Other misc code cleanups

## v0.1.6

- Fixes
    - Fixed a typo in config.h ARCH_NAME value
    - Fixed tab completion not working when cursor was on space char after a token
    - Uptime command days counter resets after every 7th day
- Developer Infos
    - Added new char library
        - bool is_digit(const char c);
        - bool is_alpha(const char c);
        - bool is_upper_alpha(const char c);
        - bool is_lower_alpha(const char c);
        - bool is_whitespace(const char c);
        - char to_upper(const char c);
        - char to_lower(const char c);
    - Improved string library
        - char* str_trim_start(const char* text);
        - char* str_trim_end(const char* text);
        - char* str_trim(const char* text);
        - char* str_to_upper(const char* text);
        - char* str_to_lower(const char* text);
    - Improved math library
        - uint64_t abs(const int64_t n);
        - int64_t clamp(const int64_t n, const int64_t min, const int64_t max);
        - bool is_power_of_two(const int64_t n);
    - Sorted files in Types folder in subfolders
    - Fixed compiler warninigs

## v0.1.5

- Cursor now blinks
    - Tick interval configureable as CURSOR_BLINK_INTERVAL (default 0,4sec)

## v0.1.4

- Stability Update
    - Fixed known bugs
        - Input buffer not accepting text after running a command
        - Protected prompt chars could be overwritten
        - History navigation leaving ghost characters on screen when cursor was mid-line
        - Tab completion not working after running a command
        - Stray character rendered at end of line after scrolling output
        - Ghost cursor appearing one line above after scroll
        - Arrow Down not fully clearing input when returning to empty line
        - Tab completion only worked for last token
    - Fixed other minor bugs

## v0.1.3

- Improved Screenbuffer Performance
- Allow Cursor movement with Arrow Right/Left keys
- Fixed and improved Cursor rendering - added CURSOR_HEIGHT config
- C++ Compiler Flags
    - O2 optimisation instead with O0
    - faster compile times due to pipe flag
- GitHub
    - Added Dependabot
        - daily interval
    - Reworked GitHub Actions

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
