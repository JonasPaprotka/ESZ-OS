#pragma once

#include <stdint.h>

#pragma region COMMAND CODES
const uint8_t IDENTIFY_DEVICE = 0xEC;
const uint8_t READ_DMA_EXT = 0x25;
const uint8_t WRITE_DMA_EXT = 0x35;
const uint8_t FLUSH_EXT = 0xEA;
#pragma endregion COMMAND CODES


#pragma region IDENTIFY
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
#pragma endregion IDENTIFY


#pragma region FIS
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
#pragma endregion FIS


#pragma region PORT STATUS BITS
struct TFD_STS_Bits {
    uint8_t ERR : 1; // Error occurred
    uint8_t Obs1 : 1; // obsolete
    uint8_t DRQ : 1; // Data Request - ready to transfer
    uint8_t Obs2 : 3; // obsolete
    uint8_t DRDY : 1; // Device Ready
    uint8_t BSY : 1; // Busy
} __attribute__((packed));

struct TFD_ERR_Bits {
    uint8_t AMNF : 1; // Address Mark Not Found (obsolete)
    uint8_t NM : 1; // No Media
    uint8_t ABRT : 1; // Command Aborted
    uint8_t MCR : 1; // Media Change Request (obsolete)
    uint8_t IDNF : 1; // ID Not Found
    uint8_t MC : 1; // Media Changed (obsolete)
    uint8_t UNC : 1; // Uncorrectable Error
    uint8_t ICRC : 1; // Interface CRC Error
} __attribute__((packed));

struct TFD_Bits {
    TFD_STS_Bits STS;
    TFD_ERR_Bits ERR;
    uint16_t Reserved;
} __attribute__((packed));

struct SSTS_Bits {
    uint32_t DET : 4;
    uint32_t SPD : 4; // 1=1.5Gb/s 2=3Gb/s 3=6Gb/s
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
#pragma endregion PORT STATUS BITS


#pragma region COMMAND STRUCTURES
struct PRDT_Bits {
    uint32_t DBA : 32;
    uint32_t DBAU : 32;
    uint32_t Reserved1;
    uint32_t DBC : 22;
    uint32_t Reserved2 : 9;
    uint32_t I : 1;
} __attribute__((packed));

struct AHCI_Command_Table {
    H2D_Register_FIS CFIS;
    uint8_t CFIS_Padding[64 - sizeof(H2D_Register_FIS)]; // 64 bytes total
    uint8_t ACMD[16]; // 16 bytes
    uint8_t Reserved[48]; // 48 bytes
    PRDT_Bits PRDT[1];
} __attribute__((packed));

struct AHCI_Command_Header { // 32 bytes
    uint32_t CFL : 5;
    uint32_t A : 1;
    uint32_t W : 1; // 1=Write / 0=Read
    uint32_t P : 1;
    uint32_t R : 1;
    uint32_t B : 1;
    uint32_t C : 1;
    uint32_t Reserved1 : 1;
    uint32_t PMP : 4;
    uint32_t PRDTL : 16;
    uint32_t PRDBC;
    uint32_t CTBA;
    uint32_t CTBAU;
    uint32_t Reserved2[4];
} __attribute__((packed));
#pragma endregion COMMAND STRUCTURES


#pragma region AHCI REGISTERS
struct AHCI_Ports {
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

struct AHCI_Registers {
    uint32_t CAP;
    uint32_t GHC;
    uint32_t IS;
    uint32_t PI; // Ports Implemented bitmask
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
#pragma endregion AHCI REGISTERS

struct ReadyPort {
    volatile AHCI_Ports* PortPointer;
    uint8_t PortIndex;
};

#pragma region GLOBALS
const uint32_t SECTOR_SIZE_BYTES = 512;

bool find_ready_ports(ReadyPort outReadyPorts[], uint8_t &outReadyPortCount);
[[nodiscard]] bool init_ahci();

IDENTIFY_Response* AHCI_IDENTIFY_DEVICE(volatile AHCI_Ports* port);
void AHCI_WRTIE_DMA_EXT(volatile AHCI_Ports* port, const uint64_t writeStartLBA, const uint16_t sectorQuantity, const void* RAM_InputPtr);
void AHCI_READ_DMA_EXT(volatile AHCI_Ports* port, const uint64_t readStartLBA, const uint16_t sectorQuantity, void* RAM_OutputPtr);
void AHCI_FLUSH_CACHE_EXT(volatile AHCI_Ports* port);
#pragma endregion GLOBALS
