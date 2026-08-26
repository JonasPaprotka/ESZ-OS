#pragma once

#include <stdint.h>
#include "bitmap.h"

extern Bitmap vmm_bitmap;

[[nodiscard]] uint64_t vmm_malloc_pages(const uint64_t byteAmount);
[[nodiscard]] bool vmm_free_pages(const uint64_t virtAddr, const uint64_t byteAmount);

[[nodiscard]] bool init_vmm();
