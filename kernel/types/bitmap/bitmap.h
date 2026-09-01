#pragma once

#include <stdint.h>

struct Bitmap {
    unsigned char* bitmap;
    uint64_t count;

    void clear();
    uint64_t find_free_range(const uint64_t range, bool& success);
    void write_bits_in_range_from(const uint64_t range, const uint64_t startBit, const bool state);
};
