#include "heap.h"
#include "limine_boot.h"
#include "pmm.h"
#include "bitmap.h"

void memory_init() {
    hhdm_offset = hhdm_request.response->offset;
    get_memory_region_count();
    get_memory_regions();
    get_free_location_for_bitmap();
    init_heap_alloc();

    // pmm_malloc(4096 * 120000); // TEST HIGH UTILISATION!!!!
}
