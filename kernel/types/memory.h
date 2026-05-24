#ifndef MEMORY_H
#define MEMORY_H
#include "byte.h"

void memory_fill(void* target, byte value, int amountOfBytesToFill);
void memory_copy(void* copyTo, void* copyFrom, int amountOfBytesToCopy);

#endif
