#include <stdint.h>
#include "pci.h"
#include "ahci.h"
#include "mbr.h"
#include "info_text.h"
#include "partitioning.h"
#include "fat32.h"
#include "storage.h"

PartitionInfo activePartition;

StorageDevice massStorageDevices[32];
uint8_t amountOfMassStorageDevices = 0;

bool find_useable_storage_medium(PartitionInfo &outPartitionInfo) {
    uint8_t buffer[SECTOR_SIZE_BYTES];
    AHCI_READ_DMA_EXT(mainMassStorageDevice, 0, 1, buffer);
    PartitionInfo partitions[4];

    if (!parse_mbr(buffer, partitions)) return false;

    for (uint8_t i = 0; i < 4; i++) {
        if (partitions[i].Type == FilesystemType::FAT32) {
            outPartitionInfo = partitions[i];
            return true;
        }
    }

    PartitionInfo partition;
    partition.Type = FilesystemType::Unknown;
    outPartitionInfo = partition;
    return false;
}

bool init_filesystem() {
    if (driveIdentifyData == nullptr) return false;

    // --- SETUP PARTITIONS ---
    if (!find_useable_storage_medium(activePartition)) return false;

    switch (activePartition.Type) {
        case FilesystemType::FAT32:
            return init_fat32(activePartition.Start_LBA);
        default:
            return false; // Unsupported filesystem
    }
}
