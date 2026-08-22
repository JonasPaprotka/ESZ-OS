#include "memory_init.h"
#include "limine_boot.h"
#include "pmm_bitmap.h"
#include "pmm.h"
#include "heap.h"
#include "vmm.h"

bool memory_init() {
    hhdm_offset = hhdm_request.response->offset;

    get_memory_region_count();
    get_memory_regions();
    get_free_location_for_bitmap();

    if (!init_vmm()) return false;
    if (!init_heap_alloc()) return false;

    return true;
}
