#include "memory_init.h"
#include "limine_boot.h"
#include "pmm_bitmap.h"
#include "pmm.h"
#include "heap.h"
#include "vmm.h"

void memory_init() {
    hhdm_offset = hhdm_request.response->offset;

    get_memory_region_count();
    get_memory_regions();
    get_free_location_for_bitmap();

    init_heap_alloc(); // old physcial - soon to be removed
    init_vmm();
}
