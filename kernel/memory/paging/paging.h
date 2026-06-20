#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

struct PageTableEntry {
    uint64_t Present : 1; // 1=valid entry
    uint64_t ReadWrite : 1; // 1=write / 0=read only
    uint64_t UserSupervisor : 1;
    uint64_t WriteThrough : 1;
    uint64_t CacheDisabled : 1; // 1=no caching
    uint64_t Accessed : 1;
    uint64_t Dirty : 1;
    uint64_t PageSizeOrPAT : 1;
    uint64_t Global : 1;
    uint64_t  Available : 3; // free to use bits
    uint64_t Address : 40; // phys. next address
    uint64_t Reserved : 11;
    uint64_t  NoExecute : 1; // 1=no code run on page
} __attribute__((packed));

struct PageTable {
    PageTableEntry entries[512];
} __attribute__((packed));

void map_page(const uint64_t virtualAddress, const uint64_t physicalAddress, const uint8_t flags);

#endif
