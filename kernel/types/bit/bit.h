#pragma once

#include <stdint.h>

inline void bit_set(unsigned char* bitmap, const uint64_t page) { bitmap[page / 8] |= (1 << (page % 8)); }
inline void bit_clear(unsigned char* bitmap, const uint64_t page) { bitmap[page / 8] &= ~(1 << (page % 8)); }

inline void bit_write(unsigned char* bitmap, const uint64_t page, const bool value) {
    if (value) bit_set(bitmap, page);
    else bit_clear(bitmap, page);
}

inline bool bit_read(unsigned char* bitmap, const uint64_t page) { return (bitmap[page / 8] >> (page % 8)) & 1; }
