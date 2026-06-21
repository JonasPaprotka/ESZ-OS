#ifndef AHCI_H
#define AHCI_H

#include <stdint.h>

// --- COMMAND CODES ---
const uint8_t IDENTIFY_DEVICE = 0xEC;
const uint8_t READ_DMA_EXT = 0x25;
const uint8_t WRITE_DMA_EXT = 0x35;
const uint8_t FLUSH_EXT = 0xEA;
// ---------------------

struct IDENTIFY_Response {
    uint8_t Reserved1[20]; // 20 bytes
    char SerialNumber[20]; // 20 bytes
    uint8_t Reserved2[14]; // 14 bytes
    char ModelName[40]; // 40 bytes
    uint8_t Reserved3[26]; // 26 bytes
    uint32_t AmountOfSectors_32bit; // 4 bytes
    uint8_t Reserved4[76]; // 76 bytes
    uint64_t AmountOfSectors_64bit; // 8 bytes
} __attribute__((packed));

struct H2D_Register_FIS {
    uint8_t FISType;
    uint8_t PMPort : 4;
    uint8_t Reserved1 : 3;
    uint8_t C : 1; // 1=Command Register Update
    uint8_t Command;
    uint8_t FeaturesLow;
    uint8_t LBA0;
    uint8_t LBA1;
    uint8_t LBA2;
    uint8_t Device;
    uint8_t LBA3;
    uint8_t LBA4;
    uint8_t LBA5;
    uint8_t FeaturesHigh;
    uint8_t CountLow;
    uint8_t CountHigh;
    uint8_t ICC;
    uint8_t Control;
} __attribute__((packed));

struct PRDT_Bits {
    uint32_t DBA;
    uint32_t DBAU;
    uint32_t Reserved1;
    uint32_t DBC : 22;
    uint32_t Reserved2 : 9;
    uint32_t I : 1;
} __attribute__((packed));

struct TFD_Bits {
    uint32_t STS : 8;
    uint32_t ERR : 8;
    uint32_t Reserved : 16;
} __attribute__((packed));

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

struct AHCI_Command_Table {
    H2D_Register_FIS CFIS;
    uint8_t CFIS_Padding[64 - sizeof(H2D_Register_FIS)]; // 64 bytes
    uint8_t ACMD[16]; // 16 bytes
    uint8_t Reserved[48]; // 48 bytes
    PRDT_Bits PRDT[1];
} __attribute__((packed));

struct AHCI_Command_Header { // 32 bytes
    uint32_t CFL : 5; // 5 bits
    uint32_t A : 1; // 1 bit
    uint32_t W : 1; // 1=Write / 0=Read - 1 bit
    uint32_t P : 1; // 1 bit
    uint32_t R : 1; // 1 bit
    uint32_t B : 1; // 1 bit
    uint32_t C : 1; // 1 bit
    uint32_t Reserved1 : 1; // 1 bit
    uint32_t PMP : 4; // 4 bits
    uint32_t PRDTL : 16; // 16 bits
    uint32_t PRDBC; // 32 bits
    uint32_t CTBA; // 32 bits
    uint32_t CTBAU; // 32 bits
    uint32_t Reserved2[4]; // 4*32 bits
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
    TFD_Bits TFD;
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

IDENTIFY_Response* AHCI_IDENTIFY_DEVICE(volatile AHCI_Ports* port);
void AHCI_WRTIE_DMA_EXT(volatile AHCI_Ports* port, const uint64_t writeStartLBA, const uint16_t sectorQuantity, const void* RAM_InputPtr);
void AHCI_READ_DMA_EXT(volatile AHCI_Ports* port, const uint64_t readStartLBA, const uint16_t sectorQuantity, void* RAM_OutputPtr);
void AHCI_FLUSH_CACHE_EXT(volatile AHCI_Ports* port);

void init_ahci();

#endif // AHCI_H
