#ifndef BIT_H
#define BIT_H
#include "byte.h"

void bit_set(byte* bitmap, int page);
void bit_clear(byte* bitmap, int page);
void bit_write(byte* bitmap, int page, bool value);

bool bit_read(byte* bitmap, int page);


#endif
