#ifndef AHCI_H
#define AHCI_H

#include <stdint.h>

struct SSTS_Bits {
    uint32_t DET : 4;
    uint32_t SPD : 4;
    uint32_t IPM : 4;
    uint32_t Reserved : 20;
} __attribute__((packed));

struct PCMD_Bits {
    uint32_t ST : 1;
    uint32_t SUD : 1;
    uint32_t POD : 1;
    uint32_t CLO : 1;
    uint32_t FRE : 1;
    uint32_t Reserved1 : 3;
    uint32_t CCS : 5;
    uint32_t MPSS : 1;
    uint32_t FR : 1;
    uint32_t CR : 1;
    uint32_t CPS : 1;
    uint32_t PMA : 1;
    uint32_t HPCP : 1;
    uint32_t MPSP : 1;
    uint32_t CPD : 1;
    uint32_t ESP : 1;
    uint32_t FBSCP : 1;
    uint32_t APSTE : 1;
    uint32_t ATAPI : 1;
    uint32_t DLAE : 1;
    uint32_t ALPE : 1;
    uint32_t ASP : 1;
    uint32_t ICC : 4;
} __attribute__((packed));

struct AHCI_Ports { // 4352 bytes
    uint32_t CLB;
    uint32_t CLBU;
    uint32_t FB;
    uint32_t FBU;
    uint32_t IS;
    uint32_t IE;
    PCMD_Bits CMD;
    uint32_t Reserved1;
    uint32_t TFD;
    uint32_t SIG;
    SSTS_Bits SSTS;
    uint32_t SCTL;
    uint32_t SERR;
    uint32_t SACT;
    uint32_t CI;
    uint32_t SNTF;
    uint32_t FBS;
    uint32_t DEVSLP;
    uint32_t Reserved2[10];
    uint32_t Vendor_Specific[4];
} __attribute__((packed));

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
    uint32_t Reserved[53];
    AHCI_Ports Ports[32];
} __attribute__((packed));

#endif
