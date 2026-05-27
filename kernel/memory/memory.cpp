/* #include "memory.h"
#include "string.h"
#include "helper/info_text.h"
#include "bit.h"

extern char _bss_end; // linker symbol. end of the kernel in memory

unsigned int memoryRegionCount;
unsigned int pageCount;
const unsigned int pageSize = 4096; // 4KB Pages

unsigned char* pmm_bitmap = (unsigned char*) &_bss_end; // bitmap lives right after kernel


void get_memory_region_count() {
    // counter with 4 bytes at 0x8000
    unsigned int *memoryRegionCountPtr = (unsigned int *) 0x8000;
    memoryRegionCount = *memoryRegionCountPtr;
}

void reserve_special_addresses() {
    // Page0: 0x0000
    // Page1-Kernel: 0x1000 – _bss_end
    // Bitmap: _bss_end – _bss_end + pageCount/8
    // -> Page0 til end of Bitmap

    const unsigned int bitmapEndPage = ((unsigned int) &_bss_end + pageCount / 8) / pageSize;
    for (unsigned i = 0; i < bitmapEndPage; i++) {
        bit_write(pmm_bitmap, i, true);
    }

    // Memory Map: Page8 0x8000/4096
    bit_write(pmm_bitmap, 8, true);

    // Stack: Pages140–144
    for (int i = 140; i < 144; i++) {
        bit_write(pmm_bitmap, i, true);
    }
}

void get_memory_regions() {
    // 8 + 8 + 4 + 4 bytes per region
    // array starts at 0x8004
    
    MemoryRegionInfo *memoryRegionInfoPtr = (MemoryRegionInfo *) 0x8004;
    
    for (unsigned int i = 0; i < memoryRegionCount; i++) {
        unsigned long long ramRegionByteAmount = memoryRegionInfoPtr[i].ramRegionLength;

        while (ramRegionByteAmount >= pageSize) {
            if (memoryRegionInfoPtr[i].ramRegionType == 1) { // type 1 = useable
                bit_write(pmm_bitmap, pageCount, false); // 0 = free
            } else {
                bit_write(pmm_bitmap, pageCount, true); // 1 = used
            } 
            ramRegionByteAmount -= pageSize;
            pageCount++;
        }
    }

    reserve_special_addresses();
}

int get_free_ram_in_kb() {
    unsigned int freePages = 0;
    for (unsigned int i = 0; i < pageCount; i++) {
        if (!bit_read(pmm_bitmap, i)) freePages++;
    }
    return freePages * 4;
}

void printMemoryInfo() {
    //printInfoLine(Info, str_combine("Memory regions detected: ", to_string(memoryRegionCount)));
    //printInfoLine(Info, str_combine("Available RAM (KB): ", to_string(get_free_ram_in_kb())));
    //printInfoLine(Info, str_combine("Available RAM (MB): ", to_string(get_free_ram_in_kb() / 1024)));
}

unsigned int calculateReqPages(const unsigned int byteAmount) {
    // round up to next page if needed
    return (byteAmount + pageSize - 1) / pageSize;
}

void ppm_malloc_page_range(unsigned int page, const unsigned int pageAmount) {
    for (unsigned int i = 0; i < pageAmount; i++) {
        bit_write(pmm_bitmap, page, true);
        page++;
    }
}

unsigned int ppm_malloc(const unsigned int byteAmount) {
    const unsigned int reqPages = calculateReqPages(byteAmount);
    unsigned int freePageCounter = 0;
    unsigned int firstPageOfSeries = 0;

    for (unsigned int i = 0; i < pageCount; i++) {
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

void ppm_free(const unsigned int addr, const unsigned int byteAmount) {
    const unsigned int reqPages = calculateReqPages(byteAmount);
    unsigned int currPage = addr / pageSize;

    for (unsigned int i = 0; i < reqPages; i++) {
        bit_write(pmm_bitmap, currPage, false);
        currPage++;
    }
}

void memory_fill(void* target, const unsigned char value, const int amountOfBytesToFill) {
    for (int i = 0; i < amountOfBytesToFill; ++i) {
        ((unsigned char*)target)[i] = value;
    }
}

void memory_clear(void* target, const int amoutOfBytesToClear) {
    memory_fill(target, 0, amoutOfBytesToClear);
}

void memory_copy(void* copyTo, const void* copyFrom, const int amountOfBytesToCopy) {
    for (int i = 0; i < amountOfBytesToCopy; ++i) {
        ((unsigned char*)copyTo)[i] = ((unsigned char*)copyFrom)[i];
    }
}
 */

// --- INIT ---
void memory_info_init() {
/*     get_memory_region_count();
    get_memory_regions();

    printMemoryInfo(); */
}
// ------------