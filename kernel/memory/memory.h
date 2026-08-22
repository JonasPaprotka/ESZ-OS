#pragma once

#include <stdint.h>

struct MemoryBlockHeader {
    uint64_t Length;
    bool Used;
} __attribute__((packed));

void memory_fill(void* target, uint8_t value, const uint64_t n);
void memory_clear(void* target, const uint64_t n);
void memory_copy(void* copyTo, const void* copyFrom, const uint64_t n);
