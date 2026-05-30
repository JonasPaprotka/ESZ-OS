#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

void memory_fill(void* target, unsigned char value, const uint64_t amountOfBytesToFill);
void memory_clear(void* target, const uint64_t amoutOfBytesToClear);
void memory_copy(void* copyTo, const void* copyFrom, const uint64_t amountOfBytesToCopy);

void memory_info_init();

uint64_t ppm_malloc(const uint64_t byteAmount);
void ppm_malloc_page_range(uint64_t page, const uint64_t pageAmount);
void ppm_free(const uint64_t addr, const uint64_t byteAmount);

void print_memory_info();

#endif
