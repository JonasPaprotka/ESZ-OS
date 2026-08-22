#include "memory_info.h"
#include "config.h"
#include "memory.h"
#include "heap.h"
#include "pmm.h"
#include "print.h"
#include "print_helper.h"
#include "info_text.h"
#include "string.h"

#pragma region MEMORY FRAG. GAPH
void print_memory_fragmentation_graph(const uint64_t maxBlocks) {
    if (maxBlocks == 0) return; //TODO consider treating it as all blocks

    uint64_t blockCounter = 0;
    MemoryBlockHeader* currBlockPtr = heapStartPtr;
    print_inline("[");

    while (currBlockPtr < heapEndPtr)
    {
        if (currBlockPtr->Used) {
            print_inline(String("U-", currBlockPtr->Length), Color::DeepOrange);
        } else {
            print_inline(String("F-", currBlockPtr->Length), Color::Green);
        }

        blockCounter++;
        if (blockCounter >= maxBlocks) {
            print_inline("...");
            break;
        }

        // GET NEXT BLOCK
        currBlockPtr = (MemoryBlockHeader*) get_next_heap_block(currBlockPtr);

        if (currBlockPtr < heapEndPtr) {
            print_inline(", ");
        }
    }

    print_inline("]");
    newline();

    print_inline("[");
    print_inline("U", Color::DeepOrange);
    print_inline("] = Used | [");
    print_inline("F", Color::Green);
    print_inline("] = Free");
    newline();
}
#pragma endregion MEMORY FRAG. GAPH

#pragma region MEMORY INFO
void print_memory_info() {
    print_separator();

    const uint64_t KiB = totalUsableBytes / 1024;
    const uint64_t MiB = KiB / 1024;
    const uint64_t GiB = MiB / 1024;

    uint64_t freePages;
    uint64_t usedPages;
    get_pmm_page_counts(freePages, usedPages);
    //const uint64_t totalPages = freePages + usedPages;

    print("----- PHYSICAL MEMORY MANAGER -----");
    // printInfoLine(InfoTextType::Info, String("Memory regions: ", memoryRegionCount));
    // char* highestAddressText = to_string(highestAddress, 16);
    // printInfoLine(InfoTextType::Info, String("Last Physical Address: ", highestAddressText));
    // free(highestAddressText);

    // // PAGES
    // printInfoLine(InfoTextType::Info, String("Total Managed Pages: ", totalPages));
    // printInfoLine(InfoTextType::Info, String("Active Free Pages: ", freePages));
    // printInfoLine(InfoTextType::Info, String("Active Used Pages: ", usedPages));

    // RAM
    const uint64_t ramUsedBytes = usedPages * PAGE_SIZE;
    const uint64_t ramFreeBytes = freePages * PAGE_SIZE;
    const uint64_t ramUsedMiB = ramUsedBytes / 1024 / 1024;
    const uint64_t ramFreeMiB = ramFreeBytes / 1024 / 1024;
    printInfoLine(InfoTextType::Info, String("Total Available RAM: ", GiB, " GiB", " | ", MiB, " MiB"));
    printInfoLine(InfoTextType::Info, String("Used: ", ramUsedMiB, " MiB", " | ", "Free: ", ramFreeMiB, " MiB"));

    // RAM Util Percentage Bar
    char* percentBar = getPercentBar(ramUsedBytes, ramFreeBytes, RAM_UTILISATION_BAR_LENGTH, true);
    printInfoLine(InfoTextType::Info, String("RAM Utilisation: ", percentBar));
    free(percentBar);

    newline();
    print("----- VIRTUAL HEAP ALLOCATOR -----");
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
    print_memory_fragmentation_graph(MAX_FRAGMENTATION_VIEW_BLOCKS);

    print_separator();
}
#pragma endregion MEMORY INFO
