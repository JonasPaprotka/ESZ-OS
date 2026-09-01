#include "vmm.h"
#include "config.h"
#include "pmm.h"
#include "pmm_bitmap.h"
#include "bit.h"
#include "math.h"
#include "info_text.h"
#include "paging.h"

Bitmap vmm_bitmap = {};

uint64_t vmm_malloc_pages(const uint64_t byteAmount) {
    const uint64_t reqPages = divide_round_up(byteAmount, PAGE_SIZE);

    bool success = false;
    uint64_t pageRangeBegin = vmm_bitmap.find_free_range(reqPages, success);

    if (!success) return UINT64_MAX;

    vmm_bitmap.write_bits_in_range_from(reqPages, pageRangeBegin, true);

    for (uint64_t i = 0; i < reqPages; i++) {
        uint64_t currVirtAddr = ((pageRangeBegin + i) * PAGE_SIZE) + VIRTUAL_OFFSET_VMM;
        uint64_t currPhysAddr = pmm_malloc_page();

        if (currPhysAddr == UINT64_MAX) {
            vmm_bitmap.write_bits_in_range_from(reqPages, pageRangeBegin, false);
            // todo: fix theoretical rollback of pmm pages could be needed
            success = false;
            return UINT64_MAX;
        }

        if (!map_page(currVirtAddr, currPhysAddr, PAGE_FLAG_WRITE)) success = false;
    }

    if (!success) return UINT64_MAX;
    return (pageRangeBegin * PAGE_SIZE) + VIRTUAL_OFFSET_VMM;
}

bool vmm_free_pages(const uint64_t virtAddr, const uint64_t byteAmount) {
    const uint64_t reqPages = divide_round_up(byteAmount, PAGE_SIZE);
    uint64_t currPage = (virtAddr - VIRTUAL_OFFSET_VMM) / PAGE_SIZE;

    bool success = true;

    for (uint64_t i = 0; i < reqPages; i++) {
        const uint64_t currVirtAddr = virtAddr + (i * PAGE_SIZE);
        PageTableEntry *pageTableEntry = page_walk(currVirtAddr);

        if (pageTableEntry == nullptr || !pageTableEntry->Present) {
            success = false;
            continue;
        }

        bit_write(vmm_bitmap.bitmap, currPage, false);

        const uint64_t physAddr = pageTableEntry->Address << 12;
        if (!free_page(currVirtAddr)) success = false;
        if (!pmm_free(physAddr, PAGE_SIZE)) success = false;

        currPage++;
    }
    return success;
}

bool init_vmm() {
    vmm_bitmap.count = pmm_bitmap.count; // TODO maybe only free pages later?
    if (vmm_bitmap.count == 0) return false;

    vmm_bitmap.bitmap = (unsigned char*) pmm_malloc_addr(divide_round_up(vmm_bitmap.count, 8));
    if (vmm_bitmap.bitmap == nullptr) return false;

    vmm_bitmap.clear();

    return true;
}
