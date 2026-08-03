#ifndef BITMAP_H
#define BITMAP_H

extern uint64_t pageCount;

void get_memory_region_count();
void get_memory_regions();
void init_bitmap_data();
void get_free_location_for_bitmap();

#endif // BITMAP_H
