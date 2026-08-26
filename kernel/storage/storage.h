#pragma once

#include <stdint.h>
#include "partitioning.h"
#include "filesystem.h"
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

    // IDENTIFICATION
    bool Identified;
    IDENTIFY_Response IdentificationInformation;

    // PARTITIONS
    PartitionInfo Partitions[4]; //TODO GPT would require 128 - MBR only 4
    uint8_t PartitionCount;
};

extern FilesystemDriver* activeDriver;

extern StorageDevice StorageDevices[32];
extern uint8_t StorageDeviceAmount;

extern StorageDevice* selectedStorageDevice;
extern PartitionInfo* selectedPartition;

[[nodiscard]] bool init_storage();
