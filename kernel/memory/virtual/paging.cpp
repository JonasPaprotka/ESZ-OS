#include "paging.h"
#include "config.h"
#include "limine_boot.h"
#include "pmm.h"
#include "math.h"
#include "memory.h"
#include "info_text.h"

// a virtual address is 4 table indexes + an offset
// [PML4 9 bit | PDPT 9 bit | PD 9 bit | PT 9 bit | offset 12 bit]
#define TABLE_IDX_MASK 0x1FF
#define PAGE_SHIFT 12

uint16_t get_PML4_Index(const uint64_t address) {
    return (address >> PAGE_SHIFT + 9 * 3) & TABLE_IDX_MASK;
}

uint16_t get_PDPT_Index(const uint64_t address) {
    return (address >> PAGE_SHIFT + 9 * 2) & TABLE_IDX_MASK;
}

uint16_t get_PD_Index(const uint64_t address) {
    return (address >> PAGE_SHIFT + 9) & TABLE_IDX_MASK;
}

uint16_t get_PT_Index(const uint64_t address) {
    return (address >> PAGE_SHIFT) & TABLE_IDX_MASK;
}

uint64_t get_or_create_next_table(PageTable* prevTableAddr, const uint16_t Idx) {
    if (prevTableAddr->entries[Idx].Present == 1)
        return (prevTableAddr->entries[Idx].Address << PAGE_SHIFT); // page number -> phys. address

    uint64_t currRouterTableAddress = pmm_malloc_page();
    memory_clear((uint64_t*)(currRouterTableAddress + hhdm_offset), PAGE_SIZE);

    prevTableAddr->entries[Idx].Present = 1;
    prevTableAddr->entries[Idx].ReadWrite = 1;

    // entries only store the page number
    prevTableAddr->entries[Idx].Address = (uint64_t)(currRouterTableAddress >> PAGE_SHIFT);
    return currRouterTableAddress;
}

void map_pages(const uint64_t virtualAddress, const uint64_t physicalAddress, const uint8_t flags, const uint64_t requiredSize) {
    // get start of tree (PML4)
    uint64_t cr3;
    __asm__ volatile("mov %%cr3, %0" : "=r"(cr3));

    const uint16_t virtPML4Idx = get_PML4_Index(virtualAddress);
    const uint16_t virtPDPTIdx = get_PDPT_Index(virtualAddress);
    const uint16_t virtPDIdx = get_PD_Index(virtualAddress);
    const uint16_t virtPTIdx = get_PT_Index(virtualAddress);

    PageTable* startPageTable = (PageTable*)(cr3 + hhdm_offset);
    PageTable* pageTable1 = (PageTable*)(get_or_create_next_table(startPageTable, virtPML4Idx) + hhdm_offset);
    PageTable* pageTable2 = (PageTable*)(get_or_create_next_table(pageTable1, virtPDPTIdx) + hhdm_offset);
    PageTable* pageTable3 = (PageTable*)(get_or_create_next_table(pageTable2, virtPDIdx) + hhdm_offset);

    const uint64_t requiredPages = divide_round_up(requiredSize, PAGE_SIZE);
    for (uint64_t i = 0; i < requiredPages; i++) {
        const uint16_t virtPTIdxAdjusted = virtPTIdx + i;

        if (virtPTIdxAdjusted >= 512) {
            printInfoLine(InfoTextType::Error, "map_pages: Paging Index is out of bounds");
            break; //TODO update this
        }

        // register on lowest tree level
        pageTable3->entries[virtPTIdxAdjusted].Present = 1;

        if (flags & PAGE_FLAG_WRITE) {
            pageTable3->entries[virtPTIdxAdjusted].ReadWrite = 1;
        } else pageTable3->entries[virtPTIdxAdjusted].ReadWrite = 0;

        if (flags & PAGE_FLAG_CACHE_DISABLE) {
            pageTable3->entries[virtPTIdxAdjusted].CacheDisabled = 1;
        } else pageTable3->entries[virtPTIdxAdjusted].CacheDisabled = 0;

        pageTable3->entries[virtPTIdxAdjusted].Address = ((physicalAddress + (i * PAGE_SIZE)) >> PAGE_SHIFT);

        // drop this page from the CPUs translation cache
        __asm__ volatile("invlpg (%0)" : : "r"((virtualAddress + (i * PAGE_SIZE))) : "memory");
    }
}

void map_page(const uint64_t virtualAddress, const uint64_t physicalAddress, const uint8_t flags) {
    map_pages(virtualAddress, physicalAddress, flags, PAGE_SIZE);
}
