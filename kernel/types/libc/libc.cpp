#include "memory.h"
#include <stdint.h>

extern "C" void* memcpy(void* dest, const void* src, uint64_t n) {
    memory_copy(dest, src, n);
    return dest;
}

extern "C" void* memset(void* dest, int value, uint64_t n) {
    memory_fill(dest, (uint8_t)value, n);
    return dest;
}

extern "C" void* memmove(void* dest, const void* src, uint64_t n) {
    memory_copy(dest, src, n);
    return dest;
}
