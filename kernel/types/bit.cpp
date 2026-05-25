// File updated by AI

#include "byte.h"

void bit_set(byte* bitmap, int page) {
    bitmap[page / 8] |= (1 << (page % 8));
}

void bit_clear(byte* bitmap, int page) {
    bitmap[page / 8] &= ~(1 << (page % 8));
}

void bit_write(byte* bitmap, int page, bool value) {
    if (value) bit_set(bitmap, page);
    else bit_clear(bitmap, page);
}

bool bit_read(byte* bitmap, int page) {
    return (bitmap[page / 8] >> (page % 8)) & 1;
}
