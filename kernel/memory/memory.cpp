#include "limine_boot.h"
#include "config.h"
#include "print_helper.h"
#include "memory.h"
#include "string.h"
#include "info_text.h"
#include "bit.h"
#include "math.h"
#include "print.h"
#include "print_helper.h"


#pragma region GLOBALS
uint64_t memoryRegionCount = 0;
uint64_t pageCount = 0;
uint64_t freeBytes = 0;
uint64_t highestAddress = 0;
uint64_t hhdm_offset = 0;
unsigned char* pmm_bitmap = nullptr;
MemoryBlockHeader* heapStartPtr = nullptr;
MemoryBlockHeader* heapEndPtr = nullptr;
#pragma endregion GLOBALS


#pragma region MEMORY REGIONS
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
#pragma endregion MEMORY REGIONS


#pragma region BITMAP
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

    uint64_t bitmapStartPage = (uint64_t)pmm_bitmap / PAGE_SIZE;
    uint64_t bitmapPagesCount = divide_round_up(reqSize, PAGE_SIZE);
    pmm_malloc_page_range(bitmapStartPage, bitmapPagesCount);
}
#pragma endregion BITMAP


#pragma region MEMORY INFO
void print_memory_info() {
    print_separator();

    const uint64_t KiB = freeBytes / 1024;
    const uint64_t MiB = KiB / 1024;
    const uint64_t GiB = MiB / 1024;

    uint64_t freePages;
    uint64_t usedPages;
    get_pmm_page_counts(freePages, usedPages);
    const uint64_t totalPages = freePages + usedPages;

    print("----- PHYSICAL MEMORY MANAGER (PMM) -----");
    printInfoLine(InfoTextType::Info, String("Memory regions: ", memoryRegionCount));
    char* highestAddressText = to_string(highestAddress, 16);
    printInfoLine(InfoTextType::Info, String("Last Physical Address: ", highestAddressText));
    free(highestAddressText);

    // PAGES
    printInfoLine(InfoTextType::Info, String("Total Managed Pages: ", totalPages));
    printInfoLine(InfoTextType::Info, String("Active Free Pages: ", freePages));
    printInfoLine(InfoTextType::Info, String("Active Used Pages: ", usedPages));

    // RAM
    const uint64_t ramUtilisationPct = (usedPages * 100) / totalPages;
    const uint64_t ramUsedBytes = usedPages * PAGE_SIZE;
    const uint64_t ramFreeBytes = freePages * PAGE_SIZE;
    const uint64_t ramUsedMiB = ramUsedBytes / 1024 / 1024;
    const uint64_t ramFreeMiB = ramFreeBytes / 1024 / 1024;
    printInfoLine(InfoTextType::Info, String("(Total) Available RAM: ", GiB, " GiB"));
    printInfoLine(InfoTextType::Info, String("(Total) Available RAM: ", MiB, " MiB"));
    printInfoLine(InfoTextType::Info, String("RAM Utilisation: ", ramUtilisationPct, " %"));
    printInfoLine(InfoTextType::Info, String("RAM Used: ", ramUsedMiB, " MiB"));
    printInfoLine(InfoTextType::Info, String("RAM Free: ", ramFreeMiB, " MiB"));
    printPercentBar(ramUsedBytes, ramFreeBytes, 20, true);

    newline();
    print("----- KERNEL HEAP ALLOCATOR -----");
    char* heapStartAddr = to_string((uint64_t) heapStartPtr, 16);
    printInfoLine(InfoTextType::Info, String("Heap Start Address: ", heapStartAddr));
    free(heapStartAddr);
    char* heapEndAddr = to_string((uint64_t) heapEndPtr, 16);
    printInfoLine(InfoTextType::Info, String("Heap End Address: ", heapEndAddr));
    free(heapEndAddr);

    // TODO Consider adding these:
    // - Total Heap Capacity KiB
    // - Metadata Overhead KiB and %
    // - Free Heap Space KiB
    // - Active Allocations x Blocks
    // - Fragmented Free Blocks x Blocks
    // - Avg Free Block Size KiB

    newline();
    print("--- HEAP FRAGMENTATION VIEW ---");
    print_memory_fragmentation_graph(100, true);

    print_separator();
}
#pragma endregion MEMORY INFO


#pragma region MEMORY FRAG. GAPH
void print_memory_fragmentation_graph(const uint64_t maxBlocks, const bool showSize) {
    if (maxBlocks == 0) return; //TODO consider treating it as all blocks

    uint64_t outputTextLength = maxBlocks + 5;
    if (showSize) outputTextLength += maxBlocks * sizeof(uint64_t);
    char outputText[outputTextLength];
    memory_clear(outputText, outputTextLength);

    uint64_t blockCounter = 0;
    MemoryBlockHeader* currBlockPtr = heapStartPtr;
    outputText[0] = '[';

    while (currBlockPtr < heapEndPtr)
    {
        if (currBlockPtr->Used) {
            str_add(outputText, "U");
        } else {
            str_add(outputText, "F");
        }

        if (showSize) {
            //const uint64_t KiB = currBlockPtr->Length / 1024;
            //const uint64_t MiB = KiB / 1024;
            str_add(outputText, String("-", currBlockPtr->Length));
        }

        blockCounter++;
        if (blockCounter >= maxBlocks) {
            str_add(outputText, "...");
            break;
        }

        // GET NEXT BLOCK
        currBlockPtr = (MemoryBlockHeader*) get_next_heap_block(currBlockPtr);

        if (showSize && currBlockPtr < heapEndPtr) {
            str_add(outputText, ", ");
        }
    }

    str_add(outputText, "]");
    print(outputText);
    print("Legend: [U] = Used; [F] = Free");
}
#pragma endregion MEMORY FRAG. GAPH


#pragma region PMM
void pmm_malloc_page_range(uint64_t page, const uint64_t pageAmount) {
    for (uint64_t i = 0; i < pageAmount; i++) {
        bit_write(pmm_bitmap, page, true);
        page++;
    }
}

uint64_t pmm_malloc(const uint64_t byteAmount) {
    const uint64_t reqPages = divide_round_up(byteAmount, PAGE_SIZE);
    uint64_t freePageCounter = 0;
    uint64_t firstPageOfSeries = 0;

    for (uint64_t i = 0; i < pageCount; i++) {
        if (!bit_read(pmm_bitmap, i)) {
            if (freePageCounter == 0) {
                firstPageOfSeries = i;
            }
            freePageCounter++;
            if (freePageCounter == reqPages) {
                pmm_malloc_page_range(firstPageOfSeries, reqPages);
                return firstPageOfSeries * PAGE_SIZE;
            }
        } else {
            freePageCounter = 0;
        }
    }

    printInfoLine(InfoTextType::Error, "pmm_malloc failed");
    return 0;
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

    for (uint64_t i = 0; i < pageCount; i++) {
        if (!bit_read(pmm_bitmap, i)) {
            freePageCounter++;
        } else {
            usedPageCounter++;
        }
    }
}

void* pmm_malloc_addr(const uint64_t byteAmount) {
    uint64_t phys_addr = pmm_malloc(byteAmount);
    if (phys_addr == 0) return nullptr;
    return (void*)(phys_addr + hhdm_offset);
}

void pmm_free(const uint64_t addr, const uint64_t byteAmount) {
    const uint64_t reqPages = divide_round_up(byteAmount, PAGE_SIZE);
    uint64_t currPage = addr / PAGE_SIZE;

    for (uint64_t i = 0; i < reqPages; i++) {
        bit_write(pmm_bitmap, currPage, false);
        currPage++;
    }
}
#pragma endregion PMM


#pragma region MEMORY FUNCTIONS
void memory_fill(void* target, const uint8_t value, const uint64_t n) {
    // fill 8 bytes using uint64_t to make it faster
    uint64_t* dest64 = (uint64_t*) target;
    const uint64_t count64 = n / 8;

    const uint64_t value64 = (uint64_t) value * 0x0101010101010101ULL; //[AI-Supported Code] clone 1 byte to 8

    for (uint64_t i = 0; i < count64; ++i) {
        dest64[i] = value64;
    }

    // remaining bytes if not divideable by 8
    uint8_t* dest8 = (uint8_t*) (dest64 + count64);

    for (uint64_t i = 0; i < (n % 8); ++i) {
        dest8[i] = value;
    }
}

void memory_clear(void* target, const uint64_t n) {
    memory_fill(target, 0, n);
}

void memory_copy(void* copyTo, const void* copyFrom, const uint64_t n) {
    // fill 8 bytes using uint64_t to make it faster
    uint64_t* dest64 = (uint64_t*) copyTo;
    const uint64_t* src64 = (const uint64_t*) copyFrom;
    const uint64_t count64 = n / 8;

    for (uint64_t i = 0; i < count64; ++i) {
        dest64[i] = src64[i];
    }

    // remaining bytes if not divideable by 8
    unsigned char* dest8 = (unsigned char*) (dest64 + count64);
    const unsigned char* src8 = (const unsigned char*) (src64 + count64);

    for (uint64_t i = 0; i < (n % 8); ++i) {
        dest8[i] = src8[i];
    }
}
#pragma endregion MEMORY FUNCTIONS


#pragma region HEAP
void init_heap_alloc() {
    MemoryBlockHeader* initialHeapBlockPtr = (MemoryBlockHeader*) pmm_malloc_addr(INIT_HEAP_SIZE);
    initialHeapBlockPtr->Length = INIT_HEAP_SIZE - sizeof(MemoryBlockHeader);
    initialHeapBlockPtr->Used = false;

    heapStartPtr = initialHeapBlockPtr;
    heapEndPtr = (MemoryBlockHeader*) ((char*) initialHeapBlockPtr + INIT_HEAP_SIZE);
}

MemoryBlockHeader create_mem_block(const uint64_t length, const bool used) {
    MemoryBlockHeader newHeader;
    newHeader.Length = length;
    newHeader.Used = used;
    return newHeader;
}

MemoryBlockHeader* get_next_heap_block(const MemoryBlockHeader* currMemBlock) {
    return (MemoryBlockHeader*) ((char*) currMemBlock + sizeof(MemoryBlockHeader) + currMemBlock->Length);
}

MemoryBlockHeader* get_prev_heap_block(const MemoryBlockHeader* currMemBlock) {
    // need loop through all blocks due to not knowing length of prev block

    MemoryBlockHeader* loopBlockPtr = heapStartPtr;
    while (loopBlockPtr >= heapStartPtr && loopBlockPtr < currMemBlock)
    {
        MemoryBlockHeader* nextBlockHeader = get_next_heap_block(loopBlockPtr);
        if (nextBlockHeader == currMemBlock) {
            return loopBlockPtr;
        }
        loopBlockPtr = nextBlockHeader;
    }

    return nullptr;
}

void* malloc(const uint64_t size) {
    MemoryBlockHeader* currBlockPtr = heapStartPtr;

    while (currBlockPtr->Length < size || currBlockPtr->Used == true)
    {
        currBlockPtr = (MemoryBlockHeader*) get_next_heap_block(currBlockPtr);
        if (currBlockPtr >= heapEndPtr) {
            printInfoLine(InfoTextType::Error, "Malloc can\'t build a suitable new block");
            return nullptr;
        }
    }

    uint64_t oldBlockLength = currBlockPtr->Length;

    currBlockPtr->Length = size;
    currBlockPtr->Used = true;

    void* returnAddr = ((char*) currBlockPtr + sizeof(MemoryBlockHeader));

    if (oldBlockLength > size + sizeof(MemoryBlockHeader)) {
        MemoryBlockHeader* nextBlock = (MemoryBlockHeader*) get_next_heap_block(currBlockPtr);
        const uint64_t nextBlockLength = oldBlockLength - size - sizeof(MemoryBlockHeader);

        // skip splitting if to small - avoid like F-1 blocks
        if (nextBlockLength <= sizeof(MemoryBlockHeader)) {
            currBlockPtr->Length = oldBlockLength;
            return returnAddr;
        }

        nextBlock->Length = nextBlockLength;
        nextBlock->Used = false;
    } else {
        currBlockPtr->Length = oldBlockLength;
    }

    return returnAddr;
}

void try_defragment_page(MemoryBlockHeader* freedBlockPtr) {
    MemoryBlockHeader* rightBlockPtr = get_next_heap_block(freedBlockPtr);
    if (rightBlockPtr < heapEndPtr) {
        if (rightBlockPtr->Used == false) {
            freedBlockPtr->Length += rightBlockPtr->Length + sizeof(MemoryBlockHeader);
        }
    }

    MemoryBlockHeader* leftBlockPtr = get_prev_heap_block(freedBlockPtr);
    if (leftBlockPtr == nullptr) { return; }

    if (leftBlockPtr >= heapStartPtr) {
        if (leftBlockPtr->Used == false) {
            leftBlockPtr->Length += freedBlockPtr->Length + sizeof(MemoryBlockHeader);
        }
    }
}

void free(const void* ptr) {
    MemoryBlockHeader* header = (MemoryBlockHeader*) ((char*) ptr - sizeof(MemoryBlockHeader));
    header->Used = false;

    try_defragment_page(header);
}
#pragma endregion HEAP


#pragma region INIT MEMORY
void memory_info_init() {
    hhdm_offset = hhdm_request.response->offset;
    get_memory_region_count();
    get_memory_regions();
    get_free_location_for_bitmap();
    init_heap_alloc();

    //pmm_malloc(4096 * 120000); // TEST HIGH UTILISATION!!!!
}
#pragma endregion INIT MEMORY
