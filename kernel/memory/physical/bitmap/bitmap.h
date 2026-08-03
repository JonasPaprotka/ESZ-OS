#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

extern uint64_t pageCount;

void get_free_location_for_bitmap();
void init_bitmap_data();

#endif // BITMAP_H
