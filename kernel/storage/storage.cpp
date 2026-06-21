#include "pci.h"
#include "ahci.h"
#include "mbr.h"
#include "info_text.h"
#include "partitioning.h"

void find_useable_storage_medium() {
    uint8_t buffer[512];
    AHCI_READ_DMA_EXT(mainMassStorageDevice, 0, 1, buffer);
    PartitionInfo partitions[4];
    parse_mbr(buffer, partitions);

    uint32_t fat32PartitionStartLBA;

    for (int i = 0; i < 4; i++) {
        if (partitions[i].Type == FilesystemType::FAT32 && partitions[i].Bootable == false) {
            fat32PartitionStartLBA = partitions[i].Start_LBA;
            printInfoLine(InfoTextType::Debug, String("FAT32 Partition found at index ", i));
        }
    }
}

void init_storage() {
    // SETUP DEVICE
    init_pci();
    init_ahci();

    // SETUP PARTITIONS
    find_useable_storage_medium();
}
