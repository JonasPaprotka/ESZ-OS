#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

extern uint64_t hhdm_offset;

struct MemoryBlockHeader {
    uint64_t Length;
    bool Used;
} __attribute__((packed));


#pragma region MEMORY REGIONS
void get_memory_region_count();
void get_memory_regions();
void init_bitmap_data();
void get_free_location_for_bitmap();
#pragma endregion MEMORY REGIONS


#pragma region MEMORY INFO
void memory_info_init();
void print_memory_info();
void print_memory_fragmentation_graph(const uint64_t maxBlocks, const bool showSize);
#pragma endregion MEMORY INFO


#pragma region PMM
void get_pmm_page_counts(uint64_t &freePageCounter, uint64_t &usedPageCounter);
uint64_t pmm_malloc(const uint64_t byteAmount);
uint64_t pmm_malloc_page();
uint64_t pmm_malloc_pages(const uint64_t pageAmount);
void* pmm_malloc_addr(const uint64_t byteAmount);
void pmm_malloc_page_range(uint64_t page, const uint64_t pageAmount);
void pmm_free(const uint64_t addr, const uint64_t byteAmount);
#pragma endregion PMM


#pragma region MEMORY FUNCTIONS
void memory_fill(void* target, uint8_t value, const uint64_t n);
void memory_clear(void* target, const uint64_t n);
void memory_copy(void* copyTo, const void* copyFrom, const uint64_t n);
#pragma endregion MEMORY FUNCTIONS

#pragma region HEAP
void init_heap_alloc();
MemoryBlockHeader create_mem_block(const uint64_t length, const bool used);
MemoryBlockHeader* get_next_heap_block(const MemoryBlockHeader* currMemBlock);
MemoryBlockHeader* get_prev_heap_block(const MemoryBlockHeader* currMemBlock);
void* malloc(const uint64_t size);
void try_defragment_page(MemoryBlockHeader* freedBlockPtr);
void free(const void* ptr);
#pragma endregion HEAP


#endif // MEMORY_H
