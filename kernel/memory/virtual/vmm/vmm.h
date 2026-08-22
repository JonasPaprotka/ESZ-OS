#pragma once

#include <stdint.h>
#include "bitmap.h"

extern Bitmap vmm_bitmap;

uint64_t vmm_malloc_pages(const uint64_t byteAmount);
bool vmm_free_pages(const uint64_t virtAddr, const uint64_t byteAmount);

bool init_vmm();
