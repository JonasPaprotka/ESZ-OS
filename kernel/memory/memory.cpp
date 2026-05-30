#include "limine_boot.h"

#include "memory.h"
#include "string.h"
#include "info_text.h"
#include "bit.h"
#include "math.h"

uint64_t memoryRegionCount = 0;
uint64_t pageCount = 0;
const uint64_t pageSize = 4096; // 4KiB Pages
uint64_t freeBytes = 0;
uint64_t highestAddress = 0;

uint64_t hhdm_offset;

unsigned char* pmm_bitmap;


void get_memory_region_count() {
    memoryRegionCount = memmap_request.response->entry_count;
}

void get_memory_regions() {
    for (uint64_t i = 0; i < memoryRegionCount; i++) {
        struct limine_memmap_entry* entry = memmap_request.response->entries[i];

        if (entry->type == LIMINE_MEMMAP_USABLE) {
            uint64_t length = entry->length;
            freeBytes += length;
            uint64_t currAddr = entry->base + length;
            if (currAddr > highestAddress) {
                highestAddress = currAddr;
            }
        }
    }
}

void init_bitmap_data() {
    for (uint64_t i = 0; i < memoryRegionCount; i++) {
        struct limine_memmap_entry* entry = memmap_request.response->entries[i];

        if (entry->type == LIMINE_MEMMAP_USABLE) {
            ppm_free(entry->base, entry->length);
        }
    } 
}

void get_free_location_for_bitmap() {
    pageCount = highestAddress / pageSize;
    uint64_t reqSize = divideRoundUp1(pageCount, 8);

    for (uint64_t i = 0; i < memoryRegionCount; i++) {
        struct limine_memmap_entry* entry = memmap_request.response->entries[i];

        if (entry->type == LIMINE_MEMMAP_USABLE) {
            if (entry->length >= reqSize) {
                pmm_bitmap = (unsigned char*) entry->base + hhdm_offset;
                break;
            }
        }
    }

    memory_fill(pmm_bitmap, 0xFF, reqSize); // mark everything as filled
    init_bitmap_data();
    
    uint64_t bitmapStartPage = (uint64_t)pmm_bitmap / pageSize;
    uint64_t bitmapPagesCount = divideRoundUp1(reqSize, pageSize);
    ppm_malloc_page_range(bitmapStartPage, bitmapPagesCount);
}

void print_memory_info() {
    uint64_t KiB = freeBytes / 1024;
    uint64_t MiB = KiB / 1024;
    uint64_t GiB = MiB / 1024;

    printInfoLine(InfoTextType::Info, str_combine("Memory regions: ", to_string(memoryRegionCount)));
    
    if (GiB > 0) {
        printInfoLine(InfoTextType::Info, str_combine(str_combine("Available RAM: ", to_string(GiB)), " GiB"));
    } else if (MiB > 0) {
        printInfoLine(InfoTextType::Info, str_combine(str_combine("Available RAM: ", to_string(MiB)), " MiB"));
    } else {
        printInfoLine(InfoTextType::Info, str_combine(str_combine("Available RAM: ", to_string(KiB)), " KiB"));
    }
}

void ppm_malloc_page_range(uint64_t page, const uint64_t pageAmount) {
    for (uint64_t i = 0; i < pageAmount; i++) {
        bit_write(pmm_bitmap, page, true);
        page++;
    }
}

uint64_t ppm_malloc(const uint64_t byteAmount) {
    const uint64_t reqPages = divideRoundUp1(byteAmount, pageSize);
    uint64_t freePageCounter = 0;
    uint64_t firstPageOfSeries = 0;

    for (uint64_t i = 0; i < pageCount; i++) {
        if (!bit_read(pmm_bitmap, i)) {
            if (freePageCounter == 0) {
                firstPageOfSeries = i;
            }
            freePageCounter++;
            if (freePageCounter == reqPages) {
                ppm_malloc_page_range(firstPageOfSeries, reqPages);
                return firstPageOfSeries * pageSize;
            }
        } else {
            freePageCounter = 0;
        }
    }

    printInfoLine(InfoTextType::Error, "ppm_malloc failed");
    return 0;
}

void* ppm_malloc_addr(const uint64_t byteAmount) {
    uint64_t phys_addr = ppm_malloc(byteAmount);
    if (phys_addr == 0) return nullptr;
    return (void*)(phys_addr + hhdm_offset); 
}

void ppm_free(const uint64_t addr, const uint64_t byteAmount) {
    const uint64_t reqPages = divideRoundUp1(byteAmount, pageSize);
    uint64_t currPage = addr / pageSize;

    for (uint64_t i = 0; i < reqPages; i++) {
        bit_write(pmm_bitmap, currPage, false);
        currPage++;
    }
}

void memory_fill(void* target, const unsigned char value, const uint64_t amountOfBytesToFill) {
    for (uint64_t i = 0; i < amountOfBytesToFill; ++i) {
        ((unsigned char*)target)[i] = value;
    }
}

void memory_clear(void* target, const uint64_t amoutOfBytesToClear) {
    memory_fill(target, 0, amoutOfBytesToClear);
}

void memory_copy(void* copyTo, const void* copyFrom, const uint64_t amountOfBytesToCopy) {
    for (uint64_t i = 0; i < amountOfBytesToCopy; ++i) {
        ((unsigned char*)copyTo)[i] = ((unsigned char*)copyFrom)[i];
    }
}


// --- INIT ---
void memory_info_init() {
    hhdm_offset = hhdm_request.response->offset;

    get_memory_region_count();
    get_memory_regions();
    get_free_location_for_bitmap();

    print_memory_info();
}
// ------------
