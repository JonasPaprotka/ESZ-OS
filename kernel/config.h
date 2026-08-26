#pragma once

#include <stdint.h>

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
constexpr uint64_t CURSOR_HEIGHT = 2;
constexpr uint64_t CURSOR_BLINK_INTERVAL = 400; // in ms
// ---------------------

// --- FONT CONFIG ---
constexpr uint64_t FONT_SIZE = 1;
// -------------------

// --- VERSION TRACKER ---
#define OS_VERSION_MAJOR 0
#define OS_VERSION_MINOR 4
#define OS_VERSION_PATCH 1
// -----------------------

// --- TERMINAL CONFIG ---
constexpr uint64_t TERMINAL_BUFFER_SIZE = 256;
constexpr uint64_t MAX_COMMAND_HISTORY = 100;
constexpr uint64_t SCROLL_HISTORY_LINES = 100;
// -----------------------

// --- MEMORY CONFIG ---
constexpr uint64_t PAGE_SIZE = 4096;
constexpr uint64_t INIT_HEAP_SIZE = 4096 * 1024 * 4; // 16 MiB
constexpr uint64_t VIRTUAL_OFFSET_MMIO = 0xFFFFA00000000000;
constexpr uint64_t VIRTUAL_OFFSET_VMM = 0xFFFFB00000000000;
// ---------------------

// --- TIMER CONFIG ---
constexpr uint64_t  PIT_TICK_HZ = 100; // timer percision in Hz
// --------------------

// --- PATH CONFIG ---
constexpr uint64_t  MAX_PATH_LENGTH = 4096; // chars
constexpr uint64_t  MAX_PATH_DEPTH = 64; // segments
// -------------------


#pragma region COMMANDS
// --- MEMORY INFO ---
constexpr uint64_t MAX_FRAGMENTATION_VIEW_BLOCKS = 100;
constexpr uint64_t RAM_UTILISATION_BAR_LENGTH = 16; // i would reccomend keeping it devideable by 2
// -------------------

#pragma endregion COMMANDS


// --- OTHER ---
#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)

#define OS_VERSION_STRING \
    "v" \
    STRINGIFY(OS_VERSION_MAJOR) "." \
    STRINGIFY(OS_VERSION_MINOR) "." \
    STRINGIFY(OS_VERSION_PATCH)
