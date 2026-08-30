#pragma once

#include <stdint.h>
#include "bitmap.h"

extern Bitmap pmm_bitmap;

extern uint64_t memoryRegionCount;
extern uint64_t totalUsableBytes;
extern uint64_t highestAddress;

void get_memory_region_count();
void get_memory_regions();

void get_pmm_page_counts(uint64_t &freePageCounter, uint64_t &usedPageCounter);

uint64_t pmm_malloc(const uint64_t byteAmount);
uint64_t pmm_malloc_page();
uint64_t pmm_malloc_pages(const uint64_t pageAmount);
void* pmm_malloc_addr(const uint64_t byteAmount);

bool pmm_malloc_page_range(uint64_t page, const uint64_t pageAmount);
bool pmm_free(const uint64_t addr, const uint64_t byteAmount);
