#include "config.h"
#include "fat32.h"
#include <stdint.h>
#include "info_text.h"
#include "ahci.h"
#include "print.h"
#include "string.h"
#include "utf16.h"
#include "filesystem.h"

FAT32_Context context;

#pragma region Helpers
bool isLFN(FAT_Attributes_Bits attributes) {
    return attributes.ReadOnly && attributes.Hidden && attributes.System && attributes.VolumeID;
}

uint64_t get_fat_sector_to_read(const uint32_t cluster) {
    return (cluster * 4) / context.BytesPerSector + context.FatStartLBA;
}

uint64_t cluster_to_lba(const uint32_t cluster) {
    return context.DataRegionStartLBA + (cluster - 2) * context.SectorsPerCluster;
}

uint32_t get_next_cluster(const uint32_t cluster) {
    uint8_t buffer[SECTOR_SIZE_BYTES];
    const uint64_t fatSector = get_fat_sector_to_read(cluster);
    const uint64_t offsetInSector = (cluster * 4) % context.BytesPerSector;

    AHCI_READ_DMA_EXT(mainMassStorageDevice, fatSector, 1, buffer);

    uint32_t* fatAsArray = (uint32_t*) buffer;
    return fatAsArray[offsetInSector / 4] & 0x0FFFFFFF;
}
#pragma endregion Helpers

#pragma region GET ENTRIES
void get_entries_in_dir(Directory_Entry* entries, Entry* outEntries, uint32_t& outEntryCount) {
    bool gotLongName = false;
    const uint32_t maxEntryCount = context.ClusterSizeInBytes / sizeof(Directory_Entry);

    for (int i = 0; i < maxEntryCount; i++) {
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

                char* trimmed = str_trim(namePart);
                str_copy(namePart, trimmed);
                free(trimmed);

                trimmed = str_trim(extPart);
                str_copy(extPart, trimmed);
                free(trimmed);

                if (extPart[0] == 0) {
                    char* lower = str_to_lower(namePart);
                    str_copy(outEntries[outEntryCount].Name, lower);
                    free(lower);
                } else {
                    char* lower = str_to_lower(String(namePart, ".", extPart));
                    str_copy(outEntries[outEntryCount].Name, lower);
                    free(lower);
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
#pragma endregion GET ENTRIES

#pragma region FIND
void read_dir_entries(uint32_t cluster, Entry* outEntries, uint32_t& outEntryCount) {
    uint8_t tempBuffer[context.ClusterSizeInBytes];
    AHCI_READ_DMA_EXT(mainMassStorageDevice, cluster_to_lba(cluster), context.SectorsPerCluster, tempBuffer);
    get_entries_in_dir((Directory_Entry*)tempBuffer, outEntries, outEntryCount);
}

Entry find_file(const char* path) {
    char* pathSections[MAX_PATH_DEPTH];
    uint64_t splitCount = 0;
    str_split(path, '/', pathSections, splitCount);

    uint32_t currCluster = context.RootCluster;

    for (uint64_t i = 0; i < splitCount; i++) {
        Entry entries[context.ClusterSizeInBytes / sizeof(Directory_Entry)];
        uint32_t entryCount = 0;
        read_dir_entries(currCluster, entries, entryCount);

        bool found = false;
        for (uint32_t j = 0; j < entryCount; j++) {
            if (entries[j].IsDirectory && str_equal(entries[j].Name, pathSections[i])) {
                currCluster = entries[j].FirstCluster;
                found = true;
                break;
            }
        }

        if (!found) {
            Entry empty = {};
            empty.Found = false;
            return empty;
        }
    }

    Entry entries[context.ClusterSizeInBytes / sizeof(Directory_Entry)];
    uint32_t entryCount = 0;
    read_dir_entries(currCluster, entries, entryCount);

    for (uint32_t j = 0; j < entryCount; j++) {
        if (!entries[j].IsDirectory && str_equal(entries[j].Name, pathSections[splitCount])) {
            entries[j].Found = true;
            return entries[j];
        }
    }

    Entry empty = {};
    empty.Found = false;
    return empty;
}
#pragma endregion FIND

#pragma region READ
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

        free(tempBuffer);
    }
    return outBuffer;
}

void read_file(const char* fileName) {
    Entry foundFile = find_file(fileName);
    if (!foundFile.Found) {
        printInfoLine(InfoTextType::Error, String("File not found: ", fileName));
        return;
    }

    uint8_t* fileData = read_cluster_chain(foundFile.FirstCluster, foundFile.Size);
    fileData[foundFile.Size] = 0;

    print((char*) fileData);
}
#pragma endregion READ

#pragma region INIT
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

    // read_file("hello.txt");
    // read_file("folder/sub/deep/test.txt");
}
#pragma endregion INIT
