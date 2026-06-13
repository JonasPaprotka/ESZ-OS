#ifndef CONFIG_H
#define CONFIG_H

#define OS_NAME "ESZ-OS"
#define OS_AUTHOR "Jonas Paprotka"
#define ARCH_NAME "x84_64"

// --- ONLY COMMENT IN ONE ---
#define KEYBOARD_LAYOUT_US
//#define KEYBOARD_LAYOUT_DE
// ---------------------------

#define OS_VERSION_MAJOR 0
#define OS_VERSION_MINOR 1
#define OS_VERSION_PATCH 1

#define TERMINAL_BUFFER_SIZE 256
#define MAX_COMMAND_HISTORY 100
#define SCROLL_HISTORY_LINES 100

#define PAGE_SIZE 4096 
#define INIT_HEAP_SIZE 4096 * 1024 * 4 // 16 MiB

#define FONT_SIZE 1

#define PIT_TICK_HZ 100 // timer percision in Hz


// MAKROS
#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)
// ------

#define OS_VERSION_STRING \
    "v" \
    STRINGIFY(OS_VERSION_MAJOR) "." \
    STRINGIFY(OS_VERSION_MINOR) "." \
    STRINGIFY(OS_VERSION_PATCH)

#endif
