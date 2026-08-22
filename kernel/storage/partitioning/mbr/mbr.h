#pragma once

#include <stdint.h>
#include "partitioning.h"

const uint8_t MBR_PARTITION_TYPE_FAT16_SMALL = 0x04;
const uint8_t MBR_PARTITION_TYPE_FAT12 = 0x01;
const uint8_t MBR_PARTITION_TYPE_FAT16 = 0x06;
const uint8_t MBR_PARTITION_TYPE_NTFS = 0x07;
const uint8_t MBR_PARTITION_TYPE_LINUX_NATIVE = 0x83;
const uint8_t MBR_PARTITION_TYPE_FAT32_CHS = 0x0B;
const uint8_t MBR_PARTITION_TYPE_FAT32_LBA = 0x0C;
const uint8_t MBR_PARTITION_TYPE_GPT_PROTECTIVE = 0xEE;
const uint8_t MBR_PARTITION_TYPE_EMPTY = 0x00;

const uint16_t MBR_PARTITION_TABLE_OFFSET = 0x1BE; // 446 bytes

struct Partition_Entry { // 16 byte
    uint8_t Boot_Flag; // 0x80=bootable, 0x00=non-bootable
    uint8_t CHS_Address_Start[3];
    uint8_t Partition_Type; // 0x0B/0x0C=FAT32, 0xEE=Protective MBR/GPT, 0x00=empty
    uint8_t CHS_Address_End[3];
    uint32_t Start_LBA;
    uint32_t Sector_Amount; // partition size
} __attribute__((packed));

bool parse_mbr(const uint8_t* sectorBuffer, PartitionInfo* outPartitions);
