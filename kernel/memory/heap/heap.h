#pragma once

#include <stdint.h>
#include "memory.h"

extern MemoryBlockHeader* heapStartPtr;
extern MemoryBlockHeader* heapEndPtr;

MemoryBlockHeader* get_next_heap_block(const MemoryBlockHeader* currMemBlock);

[[nodiscard]] void* malloc(const uint64_t size);
void free(const void* ptr);

[[nodiscard]] bool init_heap_alloc();
