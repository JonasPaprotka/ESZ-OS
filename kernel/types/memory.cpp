#include "byte.h"

void memory_fill(void* target, const byte value, const int amountOfBytesToFill) {
    for (int i = 0; i < amountOfBytesToFill; ++i) {
        ((byte*)target)[i] = value;
    }
}

void memory_clear(void* target, const int amoutOfBytesToClear) {
    memory_fill(target, 0, amoutOfBytesToClear);
}

void memory_copy(void* copyTo, const void* copyFrom, const int amountOfBytesToCopy) {
    for (int i = 0; i < amountOfBytesToCopy; ++i) {
        ((byte*)copyTo)[i] = ((byte*)copyFrom)[i];
    }
}
