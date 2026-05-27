#ifndef MEMORY_H
#define MEMORY_H

void memory_fill(void* target, unsigned char value, const int amountOfBytesToFill);
void memory_clear(void* target, const int amoutOfBytesToClear);
void memory_copy(void* copyTo, const void* copyFrom, const int amountOfBytesToCopy);

struct MemoryRegionInfo {
    unsigned long long ramRegionStartAddress;
    unsigned long long ramRegionLength;
    unsigned int ramRegionType;
    unsigned int ACPIExtension;
} __attribute__((packed));

void memory_info_init();

unsigned int ppm_malloc(const unsigned int pageAmount);
void ppm_free(const unsigned int addr, const unsigned int byteAmount);

void printMemoryInfo();

#endif
