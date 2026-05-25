#include "memory_info.h"
#include "byte.h"
#include "vga.h"
#include "string.h"
#include "info_text.h"

unsigned int memoryRegionCount;

void memory_info_init() {
    // create inintial map of memory regions
    
    // counter with 4 bytes at 0x8000
    unsigned int *memoryRegionCountPtr = (unsigned int *) 0x8000;
    memoryRegionCount = *memoryRegionCountPtr;
    printInfoLine(Info, str_combine("Memory regions detected: ", to_string(memoryRegionCount)));

    // 8+8+4+4 bytes per region. Array starts at 0x8004
}
