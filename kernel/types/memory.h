#ifndef MEMORY_H
#define MEMORY_H
#include "byte.h"

void memory_fill(void* target, byte value, const int amountOfBytesToFill);
void memory_clear(void* target, const int amoutOfBytesToClear);
void memory_copy(void* copyTo, const void* copyFrom, const int amountOfBytesToCopy);

#endif
