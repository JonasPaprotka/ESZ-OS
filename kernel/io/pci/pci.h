#pragma once

#include <stdint.h>

struct PCI_Memory_Space_BAR_MMIO { // 32 bits
    uint32_t RegionType : 1; // 1 bit
    uint32_t MemoryType : 2; // 2 bits
    uint32_t Prefetchable : 1; // 1 bit
    uint32_t BaseAddress : 28; // 28 bits
} __attribute__((packed));

struct PCI_IO_Space_BAR { // 32 bits
    uint32_t RegionType : 1; // 1 bit
    uint32_t Reserved : 1; // 1 bit
    uint32_t BaseAddress : 30; // 30 bits
} __attribute__((packed));

union PCI_BAR {
    uint32_t raw;
    PCI_Memory_Space_BAR_MMIO mem;
    PCI_IO_Space_BAR io;
} __attribute__((packed));

struct BIST_Register { // 8 bits
    uint8_t CompetionCode : 4; // 4 bits
    uint8_t Reserved : 2; // 2 bits
    uint8_t Start_BIST : 1; // 1 bit
    uint8_t BIST_Capable : 1; // 1 bit
} __attribute__((packed));


struct PCI_Device {
    uint16_t VendorID;
    uint16_t DeviceID;
    uint16_t Command;
    uint16_t Status;

    uint8_t  RevisionID;
    uint8_t  ProgIF;
    uint8_t  Subclass;
    uint8_t  ClassCode;

    uint8_t  CacheLineSize;
    uint8_t  LatencyTimer;
    uint8_t  HeaderType;
    BIST_Register  BIST; // 8bits

    PCI_BAR BAR[6]; // 6*32 bits

    uint32_t CardbusCISPointer;
    uint16_t SubsystemVendorID;
    uint16_t SubsystemID;
    uint32_t ExpansionROMBaseAddress;

    uint8_t  CapabilitiesPointer;
    uint8_t  Reserved[7];

    uint8_t  InterruptLine;
    uint8_t  InterruptPin;
    uint8_t  MinGnt;
    uint8_t  MaxLat;
} __attribute__((packed));


extern PCI_Device* found_pci_devices;
extern uint32_t PCIDeviceAmount;

bool init_pci();
