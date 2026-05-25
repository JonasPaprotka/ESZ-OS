#include "memory_info.h"
#include "byte.h"
#include "vga.h"
#include "string.h"
#include "info_text.h"
#include "bit.h"

extern char _bss_end; // linker symbol. end of the kernel in memory

unsigned int memoryRegionCount;
int pageCount;

byte* pmm_bitmap = (byte*) &_bss_end; // bitmap lives right after kernel


void get_memory_region_count() {
    // counter with 4 bytes at 0x8000
    unsigned int *memoryRegionCountPtr = (unsigned int *) 0x8000;
    memoryRegionCount = *memoryRegionCountPtr;
    printInfoLine(Info, str_combine("Memory regions detected: ", to_string(memoryRegionCount)));
}

void get_memory_regions() {
    // 8 + 8 + 4 + 4 bytes per region
    // array starts at 0x8004
    
    MemoryRegionInfo *memoryRegionInfoPtr = (MemoryRegionInfo *) 0x8004;
    
    for (int i = 0; i < memoryRegionCount; i++) {
        unsigned long long ramRegionByteAmount = memoryRegionInfoPtr[i].ramRegionLength;

        while (ramRegionByteAmount >= 4096) {
            if (memoryRegionInfoPtr[i].ramRegionType == 1) { // type 1 = useable
                bit_write(pmm_bitmap, pageCount, false); // 0 = free
            } else {
                bit_write(pmm_bitmap, pageCount, true); // 1 = used
            }
            ramRegionByteAmount -= 4096; // 4KB Pages
            pageCount++;
        }
    }
}

int get_free_ram_in_kb() {
    int freePages = 0;
    for (int i = 0; i < pageCount; i++) {
        if (!bit_read(pmm_bitmap, i)) freePages++;
    }
    return freePages * 4;
}

// --- INIT ---
void memory_info_init() {
    get_memory_region_count();
    get_memory_regions();
    printInfoLine(Info, to_string(get_free_ram_in_kb()));
}
