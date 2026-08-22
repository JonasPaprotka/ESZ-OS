#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

struct Bitmap {
    unsigned char* bitmap;
    uint64_t count;

    void clear();
    uint64_t find_free_range(uint64_t range, bool& success);
};

#endif // BITMAP_H
