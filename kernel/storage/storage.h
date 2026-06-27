#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>
#include "partitioning.h"
#include "ahci.h"

struct StorageDevice {
    // PORT
    volatile AHCI_Ports* Port;
    uint8_t PortIndex;
    uint32_t Signature;

    // DRIVE INFO
    char ModelName[41];
    char SerialNumber[21];
    uint64_t TotalSectors;

    // STATUS
    bool Ready;
    uint8_t SpeedGen; //1=1.5Gb/s 2=3Gb/s 3=6Gb/s
};


extern PartitionInfo activePartition;
extern StorageDevice massStorageDevices[32];
extern uint8_t amountOfMassStorageDevices;

bool init_filesystem();

#endif // STORAGE_H
