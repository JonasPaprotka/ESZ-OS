#include "pmm.h"
#include "config.h"
#include "limine_boot.h"
#include "pmm_bitmap.h"
#include "bit.h"
#include "math.h"
#include "info_text.h"

Bitmap pmm_bitmap = {};

uint64_t memoryRegionCount = 0;
uint64_t totalUsableBytes = 0;
uint64_t highestAddress = 0;

#pragma region region MEMORY REGIONS
void get_memory_region_count() {
    memoryRegionCount = memmap_request.response->entry_count;
}

void get_memory_regions() {
    for (uint64_t i = 0; i < memoryRegionCount; i++) {
        struct limine_memmap_entry* entry = memmap_request.response->entries[i];

        if (entry->type == LIMINE_MEMMAP_USABLE) {
            uint64_t length = entry->length;
            totalUsableBytes += length;
            uint64_t currAddr = entry->base + length;
            if (currAddr > highestAddress) {
                highestAddress = currAddr;
            }
        }
    }
}
#pragma endregion MEMORY REGIONS


bool pmm_malloc_page_range(uint64_t page, const uint64_t pageAmount) {
    for (uint64_t i = 0; i < pageAmount; i++) {
        bit_write(pmm_bitmap.bitmap, page, true);
        page++;
    }
    return true;
}

uint64_t pmm_malloc(const uint64_t byteAmount) {
    const uint64_t reqPages = divide_round_up(byteAmount, PAGE_SIZE);

    bool success = false;
    uint64_t pageRangeBegin = pmm_bitmap.find_free_range(reqPages, success);

    if (!success) {
        return UINT64_MAX;
    }

    if (!pmm_malloc_page_range(pageRangeBegin, reqPages)) return UINT64_MAX;
    return pageRangeBegin * PAGE_SIZE;
}

uint64_t pmm_malloc_page() {
    return pmm_malloc(PAGE_SIZE);
}

uint64_t pmm_malloc_pages(const uint64_t pageAmount) {
    return pmm_malloc(PAGE_SIZE * pageAmount);
}

void get_pmm_page_counts(uint64_t &freePageCounter, uint64_t &usedPageCounter) {
    freePageCounter = 0;
    usedPageCounter = 0;

    for (uint64_t i = 0; i < pmm_bitmap.count; i++) {
        if (!bit_read(pmm_bitmap.bitmap, i)) {
            freePageCounter++;
        } else {
            usedPageCounter++;
        }
    }
}

void* pmm_malloc_addr(const uint64_t byteAmount) {
    uint64_t phys_addr = pmm_malloc(byteAmount);
    if (phys_addr == UINT64_MAX) return nullptr;
    return (void*)(phys_addr + hhdm_offset);
}

bool pmm_free(const uint64_t addr, const uint64_t byteAmount) {
    const uint64_t reqPages = divide_round_up(byteAmount, PAGE_SIZE);
    uint64_t currPage = addr / PAGE_SIZE;

    for (uint64_t i = 0; i < reqPages; i++) {
        bit_write(pmm_bitmap.bitmap, currPage, false);
        currPage++;
    }
    return true;
}
