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

uint64_t get_start_of_tree() {
    uint64_t cr3;
    __asm__ volatile("mov %%cr3, %0" : "=r"(cr3));
    return cr3;
}
void drop_page_from_TLC(const uint64_t pageVirtualAddr) {
    __asm__ volatile("invlpg (%0)" : : "r"(pageVirtualAddr) : "memory");
}

void handle_read_write_flag(const uint8_t flags, PageTableEntry* pt3_PTI) {
    if (flags & PAGE_FLAG_WRITE) {
        pt3_PTI->ReadWrite = 1;
        return;
    }

    pt3_PTI->ReadWrite = 0;
}

void handle_cache_disable_flag(const uint8_t flags, PageTableEntry* pt3_PTI) {
    if (flags & PAGE_FLAG_CACHE_DISABLE) {
        pt3_PTI->CacheDisabled = 1;
        return;
    }

    pt3_PTI->CacheDisabled = 0;
}

uint64_t get_or_create_next_table(PageTable* prevTableAddr, const uint16_t Idx) {
    if (prevTableAddr->entries[Idx].Present == 1)
        return (prevTableAddr->entries[Idx].Address << PAGE_SHIFT); // page number -> phys. address

    uint64_t currRouterTableAddress = pmm_malloc_page();
    if (currRouterTableAddress == 0) return 0;

    memory_clear((uint64_t*)(currRouterTableAddress + hhdm_offset), PAGE_SIZE);

    prevTableAddr->entries[Idx].Present = 1;
    prevTableAddr->entries[Idx].ReadWrite = 1;

    // entries only store the page number
    prevTableAddr->entries[Idx].Address = (uint64_t)(currRouterTableAddress >> PAGE_SHIFT);
    return currRouterTableAddress;
}

PageTableEntry* page_walk(const uint64_t virtualAddress) {
    const uint16_t virtPML4Idx = get_PML4_Index(virtualAddress);
    const uint16_t virtPDPTIdx = get_PDPT_Index(virtualAddress);
    const uint16_t virtPDIdx = get_PD_Index(virtualAddress);
    const uint16_t virtPTIdx = get_PT_Index(virtualAddress);

    PageTable* startPageTable = (PageTable*)(get_start_of_tree() + hhdm_offset);

    uint64_t nextTableAddr = get_or_create_next_table(startPageTable, virtPML4Idx);
    if (nextTableAddr == 0) return nullptr;
    PageTable* pageTable1 = (PageTable*)(nextTableAddr + hhdm_offset);

    nextTableAddr = get_or_create_next_table(pageTable1, virtPDPTIdx);
    if (nextTableAddr == 0) return nullptr;
    PageTable* pageTable2 = (PageTable*)(nextTableAddr + hhdm_offset);

    nextTableAddr = get_or_create_next_table(pageTable2, virtPDIdx);
    if (nextTableAddr == 0) return nullptr;
    PageTable* pageTable3 = (PageTable*)(nextTableAddr + hhdm_offset);

    return &pageTable3->entries[virtPTIdx];
}

bool map_page(const uint64_t virtualAddress, const uint64_t physicalAddress, const uint8_t flags) {
    PageTableEntry* pt3_PTI = page_walk(virtualAddress);
    if (pt3_PTI == nullptr) return false;

    // register on lowest tree level
    pt3_PTI->Present = 1;

    handle_read_write_flag(flags, pt3_PTI);
    handle_cache_disable_flag(flags, pt3_PTI);

    pt3_PTI->Address = physicalAddress >> PAGE_SHIFT;

    drop_page_from_TLC(virtualAddress);
    return true;
}

bool map_pages(const uint64_t virtualAddress, const uint64_t physicalAddress, const uint8_t flags, const uint64_t requiredSize) {
    const uint64_t requiredPages = divide_round_up(requiredSize, PAGE_SIZE);
    uint64_t addressOffset = 0;
    bool success = true;

    for (uint64_t i = 0; i < requiredPages; i++) {
        addressOffset = i * PAGE_SIZE;
        if (!map_page(virtualAddress + addressOffset, physicalAddress + addressOffset, flags)) success = false;
    }

    return success;
}

bool free_page(const uint64_t virtualAddress) {
    PageTableEntry* pt3_PTI = page_walk(virtualAddress);
    if (pt3_PTI == nullptr) return false;
    *pt3_PTI = {};
    return true;
}

bool free_pages(const uint64_t virtualAddress, const uint64_t pages) {
    bool success = true;

    for (uint64_t i = 0; i < pages; i++) {
        if (!free_page(virtualAddress + (i * PAGE_SIZE))) success = false;
    }

    return success;
}
