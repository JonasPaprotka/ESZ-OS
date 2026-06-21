#include "pci.h"
#include "ahci.h"
#include "mbr.h"
#include "info_text.h"
#include "partitioning.h"
#include "fat32.h"

PartitionInfo activePartition;

PartitionInfo find_useable_storage_medium() {
    uint8_t buffer[SECTOR_SIZE_BYTES];
    AHCI_READ_DMA_EXT(mainMassStorageDevice, 0, 1, buffer);
    PartitionInfo partitions[4];
    parse_mbr(buffer, partitions);

    uint32_t fat32PartitionStartLBA = 0;

    for (int i = 0; i < 4; i++) {
        if (partitions[i].Type == FilesystemType::FAT32) {
            fat32PartitionStartLBA = partitions[i].Start_LBA;
            printInfoLine(InfoTextType::Info, String("FAT32 Partition found at index ", i));
            return partitions[i];
        }
    }

    printInfoLine(InfoTextType::Error, "No FAT32 partition found");

    PartitionInfo partition;
    partition.Type = FilesystemType::Unknown;
    return partition;
}

void init_storage() {
    // SETUP DEVICE
    init_pci();
    init_ahci();

    // SETUP PARTITIONS
    activePartition = find_useable_storage_medium();
    switch (activePartition.Type) {
        case FilesystemType::FAT32:
            init_fat32(activePartition.Start_LBA);
            break;
        default:
            printInfoLine(InfoTextType::Error, "Unsupported filesystem");
            break;
    }
}
