#include "config.h"
#include "memory.h"
#include "print.h"
#include "info_text.h"
#include "string.h"
#include "pmm.h"
#include "heap.h"
#include "print_helper.h"

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

#pragma region MEMORY INFO
void print_memory_info() {
    print_separator();

    const uint64_t KiB = totalUsableBytes / 1024;
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
