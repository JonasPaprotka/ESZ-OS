#include "mbr.h"
#include "partitioning.h"

bool parse_mbr(const uint8_t* sectorBuffer, PartitionInfo* outPartitions) {

    // MBR signature check 0xAA55
    if (sectorBuffer[510] != 0x55 || sectorBuffer[511] != 0xAA) {
        for (uint8_t i = 0; i < 4; i++) outPartitions[i].Type = FilesystemType::Empty;
        return false;
    }

    Partition_Entry* entries = (Partition_Entry*)(sectorBuffer + MBR_PARTITION_TABLE_OFFSET);

    for (uint8_t i = 0; i < 4; i++) {
        if (entries[i].Boot_Flag == 0x80) {
            outPartitions[i].Bootable = true;
        } else outPartitions[i].Bootable = false;

        outPartitions[i].Start_LBA = entries[i].Start_LBA;
        outPartitions[i].Sector_Count = entries[i].Sector_Amount;

        switch (entries[i].Partition_Type) {
            case MBR_PARTITION_TYPE_FAT12:
                outPartitions[i].Type = FilesystemType::FAT12;
                break;
            case MBR_PARTITION_TYPE_FAT16:
            case MBR_PARTITION_TYPE_FAT16_SMALL:
                outPartitions[i].Type = FilesystemType::FAT16;
                break;
            case MBR_PARTITION_TYPE_FAT32_CHS:
            case MBR_PARTITION_TYPE_FAT32_LBA:
                outPartitions[i].Type = FilesystemType::FAT32;
                break;
            case MBR_PARTITION_TYPE_GPT_PROTECTIVE:
                outPartitions[i].Type = FilesystemType::GptProtective;
                break;
            case MBR_PARTITION_TYPE_LINUX_NATIVE:
                outPartitions[i].Type = FilesystemType::Ext;
                break;
            case MBR_PARTITION_TYPE_NTFS:
                outPartitions[i].Type = FilesystemType::NTFS;
                break;
            case MBR_PARTITION_TYPE_EMPTY:
                outPartitions[i].Type = FilesystemType::Empty;
                break;
            default:
                outPartitions[i].Type = FilesystemType::Unknown;
                break;
        }
    }

    return true;
}
