#include "fat32.h"
#include <stdint.h>
#include "info_text.h"
#include "ahci.h"
#include "print.h"
#include "string.h"
#include "utf16.h"
#include "filesystem.h"

FAT32_Context context;

// TODO sort functions so i dont have to do this shit
uint64_t cluster_to_lba(const uint32_t cluster);
uint32_t get_next_cluster(const uint32_t cluster);
uint64_t get_fat_sector_to_read(const uint32_t cluster);
void get_file_entries(Directory_Entry* entries, FileEntry* outEntries, uint32_t& outEntryCount);

// ---- FINDS -----
FileEntry find_root_file_by_name(const char* name) {
    uint32_t currCluster = context.RootCluster;

    while (currCluster < EOC_START) {
        uint8_t tempBuffer[context.ClusterSizeInBytes];
        AHCI_READ_DMA_EXT(mainMassStorageDevice, cluster_to_lba(currCluster), context.SectorsPerCluster, tempBuffer);

        // ----- PARSE ------
        Directory_Entry* entries = (Directory_Entry*)tempBuffer;
        FileEntry fileEntries[context.ClusterSizeInBytes / sizeof(Directory_Entry)];
        memory_clear(fileEntries, sizeof(fileEntries));
        uint32_t fileEntryAmount = 0;
        get_file_entries(entries, fileEntries, fileEntryAmount);

        for (uint32_t i = 0; i < fileEntryAmount; i++) {
            // printInfoLine(InfoTextType::Debug, fileEntries[i].Name);


            if (!fileEntries[i].IsDirectory && str_equal(fileEntries[i].Name, name)) {
                fileEntries[i].Found = true;
                return fileEntries[i];
            }
        }

        currCluster = get_next_cluster(currCluster);
    }

    FileEntry empty = {};
    empty.Found = false;
    return empty;
}

uint64_t get_fat_sector_to_read(const uint32_t cluster) {
    return (cluster * 4) / context.BytesPerSector + context.FatStartLBA;
}

uint64_t cluster_to_lba(const uint32_t cluster) {
    return context.DataRegionStartLBA + (cluster - 2) * context.SectorsPerCluster;
}

uint8_t* read_cluster_chain(const uint32_t startCluster, const uint64_t size) {
    uint32_t currCluster = startCluster;
    uint64_t bufferWriteOffset = 0;
    uint8_t* outBuffer = (uint8_t*) malloc(size + 1);

    while (currCluster < EOC_START) {
        uint8_t* tempBuffer = (uint8_t*) malloc(context.ClusterSizeInBytes);

        // READ DATA
        AHCI_READ_DMA_EXT(mainMassStorageDevice, cluster_to_lba(currCluster), context.SectorsPerCluster, tempBuffer);

        memory_copy(outBuffer + bufferWriteOffset, tempBuffer, context.ClusterSizeInBytes);

        bufferWriteOffset += context.ClusterSizeInBytes;
        currCluster = get_next_cluster(currCluster);
    }

    return outBuffer;
}

uint32_t get_next_cluster(const uint32_t cluster) {
    uint8_t buffer[SECTOR_SIZE_BYTES];
    const uint64_t fatSector = get_fat_sector_to_read(cluster);
    const uint64_t offsetInSector = (cluster * 4) % context.BytesPerSector;

    AHCI_READ_DMA_EXT(mainMassStorageDevice, fatSector, 1, buffer);

    uint32_t* fatAsArray = (uint32_t*) buffer;
    return fatAsArray[offsetInSector / 4] & 0x0FFFFFFF;
}

bool isLFN(FAT_Attributes_Bits attributes) {
    return attributes.ReadOnly && attributes.Hidden && attributes.System && attributes.VolumeID;
}

void get_file_entries(Directory_Entry* entries, FileEntry* outEntries, uint32_t& outEntryCount) {
    bool gotLongName = false;

    for (int i = 0; i < 16; i++) {
        if (entries[i].Name[0] == 0) break;

        const bool isEntryLFN = isLFN(entries[i].Attributes);

        if (isEntryLFN) {
            LFN_Entry_Struct* lfn = (LFN_Entry_Struct*)&entries[i];

            // BUILD NAME
            char* part1 = utf16_text_to_ascii(lfn->NameChars1, 5);
            char* part2 = utf16_text_to_ascii(lfn->NameChars2, 6);
            char* part3 = utf16_text_to_ascii(lfn->NameChars3, 2);
            str_add(outEntries[outEntryCount].Name, String(part1, part2, part3));
            free(part1);
            free(part2);
            free(part3);

            gotLongName = true;
        } else {
            if (!gotLongName) {
                char namePart[9];
                char extPart[4];
                memory_copy(namePart, entries[i].Name, 8);
                namePart[8] = 0;

                memory_copy(extPart, entries[i].Name + 8, 3);
                extPart[3] = 0;

                str_copy(namePart, str_trim(namePart));
                str_copy(extPart, str_trim(extPart));

                if (extPart[0] == 0) {
                    str_copy(outEntries[outEntryCount].Name, namePart);
                } else {
                    str_copy(outEntries[outEntryCount].Name, String(namePart, ".", extPart));
                }
            }

            gotLongName = false;

            outEntries[outEntryCount].Size = entries[i].FileSize;
            outEntries[outEntryCount].IsDirectory = entries[i].Attributes.Directory;
            outEntries[outEntryCount].FirstCluster = ((uint32_t)entries[i].FirstClusterHigh << 16) | entries[i].FirstClusterLow;
        }

        if (!isEntryLFN) outEntryCount++;
    }
}


// --------- READ FILE -----------
void read_file(const char* fileName) {
    FileEntry foundFile = find_root_file_by_name(fileName);
    if (!foundFile.Found) {
        printInfoLine(InfoTextType::Error, String("File not found: ", fileName));
        return;
    }

    uint8_t* fileData = read_cluster_chain(foundFile.FirstCluster, foundFile.Size);
    fileData[foundFile.Size] = 0;

    print((char*) fileData);
}



void init_fat32(const uint32_t Start_LBA) {
    uint8_t buffer[SECTOR_SIZE_BYTES];
    AHCI_READ_DMA_EXT(mainMassStorageDevice, Start_LBA, 1, buffer);

    BPB* bpb = (BPB*)buffer;

    context.FatStartLBA = Start_LBA + bpb->ReservedSectors;
    context.DataRegionStartLBA = context.FatStartLBA + (bpb->AmountOfFAT_Copies * bpb->SectorsPerFAT);
    context.SectorsPerCluster = bpb->SectorsPerCluster;
    context.RootCluster = bpb->RootBegin_ClusterNumber;
    context.BytesPerSector = bpb->BytesPerSector;
    context.RootDirLBA = context.DataRegionStartLBA + (bpb->RootBegin_ClusterNumber - 2) * bpb->SectorsPerCluster;
    context.ClusterSizeInBytes = context.SectorsPerCluster * context.BytesPerSector;

    read_file("SECOND.TXT");
}
