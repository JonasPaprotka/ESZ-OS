#ifndef MEMORY_INFO_H
#define MEMORY_INFO_H

struct MemoryRegionInfo {
    unsigned long long ramRegionStartAddress;
    unsigned long long ramRegionLength;
    unsigned int ramRegionType;
    unsigned int ACPIExtension;
} __attribute__((packed));

void memory_info_init();

#endif
