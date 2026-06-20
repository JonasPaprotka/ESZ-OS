#ifndef AHCI_H
#define AHCI_H

#include <stdint.h>

struct AHCI_Registers { // 11x32 bits (44 bytes)
    uint32_t CAP;
    uint32_t GHC;
    uint32_t IS;
    uint32_t PI;
    uint32_t VS;
    uint32_t CCC_CTL;
    uint32_t CCC_PORTS;
    uint32_t EM_LOC;
    uint32_t EM_CTL;
    uint32_t CAP2;
    uint32_t BOHC;
} __attribute__((packed));

#endif
