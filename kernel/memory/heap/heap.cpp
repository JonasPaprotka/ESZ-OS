#include "heap.h"
#include "config.h"
#include "info_text.h"
#include "pmm.h"

MemoryBlockHeader* heapStartPtr = nullptr;
MemoryBlockHeader* heapEndPtr = nullptr;

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
