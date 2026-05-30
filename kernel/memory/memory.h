#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

struct MemoryBlockHeader {
    uint64_t Length;
    bool Used;
} __attribute__((packed));


void memory_fill(void* target, unsigned char value, const uint64_t amountOfBytesToFill);
void memory_clear(void* target, const uint64_t amoutOfBytesToClear);
void memory_copy(void* copyTo, const void* copyFrom, const uint64_t amountOfBytesToCopy);

void memory_info_init();

uint64_t pmm_malloc(const uint64_t byteAmount);
void* pmm_malloc_addr(const uint64_t byteAmount);

void pmm_malloc_page_range(uint64_t page, const uint64_t pageAmount);
void pmm_free(const uint64_t addr, const uint64_t byteAmount);

void* malloc(uint64_t size);
void free(void* ptr);

void print_memory_info();

#endif
