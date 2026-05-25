#ifndef MEMORY_H
#define MEMORY_H
#include "byte.h"

void memory_fill(void* target, byte value, const int amountOfBytesToFill);
void memory_clear(void* target, const int amoutOfBytesToClear);
void memory_copy(void* copyTo, const void* copyFrom, const int amountOfBytesToCopy);

struct MemoryRegionInfo {
    unsigned long long ramRegionStartAddress;
    unsigned long long ramRegionLength;
    unsigned int ramRegionType;
    unsigned int ACPIExtension;
} __attribute__((packed));

void memory_info_init();

unsigned int malloc(const unsigned int pageAmount);
void free(const unsigned int addr, const unsigned int byteAmount);

void printMemoryInfo();

#endif
