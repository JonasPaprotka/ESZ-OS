#pragma once

#include <stdint.h>
#include "memory.h"

extern MemoryBlockHeader* heapStartPtr;
extern MemoryBlockHeader* heapEndPtr;

MemoryBlockHeader create_mem_block(const uint64_t length, const bool used);
MemoryBlockHeader* get_next_heap_block(const MemoryBlockHeader* currMemBlock);
MemoryBlockHeader* get_prev_heap_block(const MemoryBlockHeader* currMemBlock);
void try_defragment_page(MemoryBlockHeader* freedBlockPtr);

[[nodiscard]] void* malloc(const uint64_t size);
void free(const void* ptr);

[[nodiscard]] bool init_heap_alloc();
