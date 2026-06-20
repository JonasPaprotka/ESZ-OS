#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

extern uint64_t hhdm_offset;

struct MemoryBlockHeader {
    uint64_t Length;
    bool Used;
} __attribute__((packed));

void get_pmm_page_counts(uint64_t &freePageCounter, uint64_t &usedPageCounter);

void memory_fill(void* target, uint8_t value, const uint64_t n);
void memory_clear(void* target, const uint64_t n);
void memory_copy(void* copyTo, const void* copyFrom, const uint64_t n);

void memory_info_init();

void print_memory_fragmentation_graph(const uint64_t maxBlocks, const bool showSize);

uint64_t pmm_malloc(const uint64_t byteAmount);
uint64_t pmm_malloc_page();
uint64_t pmm_malloc_pages(const uint64_t pageAmount);
void* pmm_malloc_addr(const uint64_t byteAmount);

void pmm_malloc_page_range(uint64_t page, const uint64_t pageAmount);
void pmm_free(const uint64_t addr, const uint64_t byteAmount);

void* malloc(const uint64_t size);
void free(const void* ptr);

void print_memory_info();

#endif // MEMORY_H
