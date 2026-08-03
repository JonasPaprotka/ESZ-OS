#include "config.h"
#include <stdint.h>
#include "limine_boot.h"
#include "pmm.h"
#include "math.h"
#include "memory.h"

uint64_t pageCount = 0;

void init_bitmap_data() {
    for (uint64_t i = 0; i < memoryRegionCount; i++) {
        struct limine_memmap_entry* entry = memmap_request.response->entries[i];

        if (entry->type == LIMINE_MEMMAP_USABLE) {
            pmm_free(entry->base, entry->length);
        }
    }
}

void get_free_location_for_bitmap() {
    pageCount = highestAddress / PAGE_SIZE;
    uint64_t reqSize = divide_round_up(pageCount, 8);
    uint64_t bitmapPhysBase = 0;

    for (uint64_t i = 0; i < memoryRegionCount; i++) {
        struct limine_memmap_entry* entry = memmap_request.response->entries[i];

        if (entry->type == LIMINE_MEMMAP_USABLE) {
            if (entry->length >= reqSize) {
                bitmapPhysBase = entry->base;
                pmm_bitmap = (unsigned char*) entry->base + hhdm_offset;
                break;
            }
        }
    }

    memory_fill(pmm_bitmap, 0xFF, reqSize); // mark everything as filled
    init_bitmap_data();

    uint64_t bitmapStartPage = bitmapPhysBase / PAGE_SIZE;
    uint64_t bitmapPagesCount = divide_round_up(reqSize, PAGE_SIZE);
    pmm_malloc_page_range(bitmapStartPage, bitmapPagesCount);
}
