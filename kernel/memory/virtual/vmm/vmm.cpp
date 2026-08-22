#include "config.h"
#include <stdint.h>
#include "pmm.h"
#include "pmm_bitmap.h"
#include "bit.h"
#include "math.h"

Bitmap vmm_bitmap = {};

bool init_vmm_bitmap() {
    vmm_bitmap.count = pmm_bitmap.count; // TODO maybe only free pages later?
    if (vmm_bitmap.count == 0) return false;

    vmm_bitmap.bitmap = (unsigned char*) pmm_malloc_addr(divide_round_up(vmm_bitmap.count, 8));
    vmm_bitmap.clear();

    return true;
}

// uint64_t vmm_malloc_page() {}

bool init_vmm() {
    return init_vmm_bitmap();
}
