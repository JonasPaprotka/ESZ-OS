#pragma once

#include <stdint.h>

enum class FilesystemType {
    Unknown,
    Empty,
    GptProtective,
    FAT12,
    FAT16,
    FAT32,
    NTFS,
    Ext
};

struct PartitionInfo {
    uint32_t Start_LBA;
    uint32_t Sector_Count;
    FilesystemType Type;
    bool Bootable;
};
