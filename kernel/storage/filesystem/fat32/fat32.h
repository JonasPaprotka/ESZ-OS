#ifndef FAT32_H
#define FAT32_H

#include <stdint.h>

struct FAT_Time_Bits {
    uint16_t Seconds_Div2 : 5;
    uint16_t Minutes : 6;
    uint16_t Hours : 5;
} __attribute__((packed));

struct FAT_Date_Bits {
    uint16_t Day : 5;
    uint16_t Month : 4;
    uint16_t Year : 7;
} __attribute__((packed));

struct FAT_Attributes_Bits {
    uint8_t ReadOnly : 1;
    uint8_t Hidden : 1;
    uint8_t System : 1;
    uint8_t VolumeID : 1;
    uint8_t Directory : 1;
    uint8_t Archive : 1;
    uint8_t Reserved : 2;
} __attribute__((packed));

struct FAT32_Context {
    uint64_t FatStartLBA;
    uint64_t DataRegionStartLBA;
    uint32_t RootCluster;
    uint8_t SectorsPerCluster;
    uint16_t BytesPerSector;
};

struct LFN_Entry_Struct {
    uint8_t SequenceNumber;
    char NameChars1[10];
    FAT_Attributes_Bits Attributes;
    uint8_t Type;
    uint8_t Checksum;
    char NameChars2[12];
    uint16_t FirstClusterLow;
    char NameChars3[4];
} __attribute__((packed));

struct Directory_Entry {
    char Name[11];
    FAT_Attributes_Bits Attributes;
    uint8_t NTReserved;
    uint8_t CreateTimeTenths;
    FAT_Time_Bits CreateTime;
    FAT_Date_Bits CreateDate;
    FAT_Date_Bits LastAccessDate;
    uint16_t FirstClusterHigh;
    FAT_Time_Bits WriteTime;
    FAT_Date_Bits WriteDate;
    uint16_t FirstClusterLow;
    uint32_t FileSize;
} __attribute__((packed));

struct BPB {
    uint8_t JumpInstruction[3];
    char OEM_Name[8];
    uint16_t BytesPerSector;
    uint8_t SectorsPerCluster;
    uint16_t ReservedSectors;
    uint8_t AmountOfFAT_Copies;
    uint16_t RootEntryCount;
    uint16_t TotalSectors16;
    uint8_t MediaType;
    uint16_t FATSize16;
    uint16_t SectorsPerTrack;
    uint16_t NumberOfHeads;
    uint32_t HiddenSectors;
    uint32_t TotalSectors32;
    uint32_t SectorsPerFAT;
    uint16_t ExtFlags;
    uint16_t FilesystemVersion;
    uint32_t RootBegin_ClusterNumber;
    uint16_t FSInfoSector;
    uint16_t BackupBootSector;
    uint8_t Reserved1[12];
    uint8_t DriveNumber;
    uint8_t Reserved2;
    uint8_t BootSignature;
    uint32_t VolumeID;
    char VolumeLabel[11];
    char FilesystemType[8];
} __attribute__((packed));

void init_fat32(const uint32_t Start_LBA);

#endif // FAT32_H
