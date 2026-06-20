#ifndef CONFIG_H
#define CONFIG_H

// --- OS INFO ---
#define OS_NAME "ESZ-OS"
#define OS_AUTHOR "Jonas Paprotka"
#define ARCH_NAME "x86_64"
// ---------------

// --- ONLY COMMENT IN ONE ---
#define KEYBOARD_LAYOUT_US
//#define KEYBOARD_LAYOUT_DE
// ---------------------------

// --- CURSOR CONFIG ---
#define CURSOR_HEIGHT 2
#define CURSOR_BLINK_INTERVAL 400 // in ms
// ---------------------

// --- FONT CONFIG ---
#define FONT_SIZE 1
// -------------------

// --- VERSION TRACKER ---
#define OS_VERSION_MAJOR 0
#define OS_VERSION_MINOR 2
#define OS_VERSION_PATCH 1
// -----------------------

// --- TERMINAL CONFIG ---
#define TERMINAL_BUFFER_SIZE 256
#define MAX_COMMAND_HISTORY 100
#define SCROLL_HISTORY_LINES 100
// -----------------------

// --- MEMORY CONFIG ---
#define PAGE_SIZE 4096
#define INIT_HEAP_SIZE 4096 * 1024 * 4 // 16 MiB
#define VIRTUAL_OFFSET_MMIO 0xFFFFA00000000000
// ---------------------

// --- TIMER CONFIG ---
#define PIT_TICK_HZ 100 // timer percision in Hz
// --------------------

// --- OTHER ---
#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)

#define OS_VERSION_STRING \
    "v" \
    STRINGIFY(OS_VERSION_MAJOR) "." \
    STRINGIFY(OS_VERSION_MINOR) "." \
    STRINGIFY(OS_VERSION_PATCH)

#endif // CONFIG_H
