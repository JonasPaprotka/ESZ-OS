#include "config.h"
#include <stdint.h>
#include "pmm.h"
#include "pmm_bitmap.h"
#include "bit.h"

Bitmap vmm_bitmap = {};

unsigned char* build_vmm_bitmap() {
    unsigned char *bitmap = nullptr;
    for (uint64_t i = 0; i < vmm_bitmap.count; i++) {
        bit_write(bitmap, i, false);
    }
    return bitmap;
}

// uint64_t vmm_malloc_page() {}

bool init_vmm() {
    vmm_bitmap.count = pmm_bitmap.count; // TODO maybe only free pages later? no big savior right now
}
