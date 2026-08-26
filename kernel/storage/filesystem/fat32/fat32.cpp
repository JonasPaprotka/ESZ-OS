#include "fat32.h"
#include "config.h"
#include "ahci.h"
#include "storage.h"
#include "math.h"
#include "memory.h"
#include "heap.h"
#include "string.h"
#include "utf16.h"

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

    AHCI_READ_DMA_EXT(selectedStorageDevice->Port, fatSector, 1, buffer);

    uint32_t* fatAsArray = (uint32_t*) buffer;
    return fatAsArray[offsetInSector / 4] & 0x0FFFFFFF;
}
#pragma endregion Helpers

#pragma region GET ENTRIES
void get_entries_in_dir(Directory_Entry* entries, Entry* outEntries, uint32_t& outEntryCount) {
    bool gotLongName = false;
    const uint32_t maxEntryCount = context.ClusterSizeInBytes / sizeof(Directory_Entry);

    for (uint32_t i = 0; i < maxEntryCount; i++) {
        if (entries[i].Name[0] == 0) break;

        if (entries[i].Name[0] == '.' && entries[i].Name[1] == ' ') continue;
        if (entries[i].Name[0] == '.' && entries[i].Name[1] == '.' && entries[i].Name[2] == ' ') continue;

        const bool isEntryLFN = isLFN(entries[i].Attributes);

        if (!isEntryLFN && entries[i].Attributes.VolumeID) continue;

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

            outEntryCount++;
        }
    }
}
#pragma endregion GET ENTRIES

#pragma region FIND
void read_dir_entries(uint32_t cluster, Entry* outEntries, uint32_t& outEntryCount) {
    if (cluster < 2 || cluster >= EOC_START) return;

    uint8_t* tempBuffer = (uint8_t*) malloc(context.ClusterSizeInBytes);
    AHCI_READ_DMA_EXT(selectedStorageDevice->Port, cluster_to_lba(cluster), context.SectorsPerCluster, tempBuffer);
    get_entries_in_dir((Directory_Entry*)tempBuffer, outEntries, outEntryCount);
    free(tempBuffer);
}

static void free_path_sections(char* pathSections[], const uint64_t count) {
    for (uint64_t i = 0; i < count; i++) {
        free(pathSections[i]);
    }
}

Entry find_entry(const char* path) {
    const char* normalizedPath = path;
    if (normalizedPath[0] == '/') {
        normalizedPath++;
    }

    // ROOT
    if (normalizedPath[0] == 0 || is_root_path(normalizedPath)) {
        Entry root = {};
        root.IsDirectory = true;
        root.FirstCluster = context.RootCluster;
        root.Found = true;
        return root;
    }

    char* pathSections[MAX_PATH_DEPTH];
    uint64_t splitCount = 0;
    str_split(normalizedPath, '/', pathSections, splitCount);

    uint32_t currCluster = context.RootCluster;
    const uint32_t maxEntries = context.ClusterSizeInBytes / sizeof(Directory_Entry);

    for (uint64_t i = 0; i + 1 < splitCount; i++) {
        Entry* entries = (Entry*) malloc(sizeof(Entry) * maxEntries);
        memory_clear(entries, sizeof(Entry) * maxEntries);

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

        free(entries);

        if (!found) {
            free_path_sections(pathSections, splitCount);
            Entry empty = {};
            empty.Found = false;
            return empty;
        }
    }

    Entry* entries = (Entry*) malloc(sizeof(Entry) * maxEntries);
    memory_clear(entries, sizeof(Entry) * maxEntries);

    uint32_t entryCount = 0;
    read_dir_entries(currCluster, entries, entryCount);

    for (uint32_t j = 0; j < entryCount; j++) {
        if (str_equal(entries[j].Name, pathSections[splitCount - 1])) {
            Entry result = entries[j];
            result.Found = true;
            free(entries);
            free_path_sections(pathSections, splitCount);
            return result;
        }
    }

    free(entries);
    free_path_sections(pathSections, splitCount);

    Entry empty = {};
    empty.Found = false;
    return empty;
}

uint64_t get_entries(const char* path, Entry*& outEntries) {
    Entry dir = find_entry(path);
    if (!dir.Found || !dir.IsDirectory) {
        outEntries = nullptr;
        return 0;
    }

    const uint32_t maxEntries = context.ClusterSizeInBytes / sizeof(Directory_Entry);
    outEntries = (Entry*) malloc(sizeof(Entry) * maxEntries);
    memory_clear(outEntries, sizeof(Entry) * maxEntries);

    uint32_t entryCount = 0;
    read_dir_entries(dir.FirstCluster, outEntries, entryCount);

    return entryCount;
}
#pragma endregion FIND

#pragma region READ
uint8_t* read_cluster_chain(const uint32_t startCluster, const uint64_t size) {
    if (startCluster < 2 || startCluster >= EOC_START) return nullptr;

    uint32_t currCluster = startCluster;
    uint64_t bufferWriteOffset = 0;
    const uint64_t numClusters = divide_round_up(size, context.ClusterSizeInBytes);
    uint8_t* outBuffer = (uint8_t*) malloc(numClusters * context.ClusterSizeInBytes + 1);

    while (currCluster >= 2 && currCluster < EOC_START) {
        uint8_t* tempBuffer = (uint8_t*) malloc(context.ClusterSizeInBytes);

        // READ DATA
        AHCI_READ_DMA_EXT(selectedStorageDevice->Port, cluster_to_lba(currCluster), context.SectorsPerCluster, tempBuffer);
        memory_copy(outBuffer + bufferWriteOffset, tempBuffer, context.ClusterSizeInBytes);

        bufferWriteOffset += context.ClusterSizeInBytes;
        currCluster = get_next_cluster(currCluster);

        free(tempBuffer);
    }
    return outBuffer;
}

uint8_t* fat32_read(const Entry& entry) {
    uint8_t* fileData = read_cluster_chain(entry.FirstCluster, entry.Size);
    fileData[entry.Size] = 0;
    return fileData;
}
#pragma endregion READ

#pragma region WRITE
uint64_t fat32_write(const Entry&, const uint8_t*, uint64_t) {
    //TODO
    return 0;
}
#pragma endregion WRITE

#pragma region INIT
bool init_fat32(const uint32_t Start_LBA) {
    uint8_t buffer[SECTOR_SIZE_BYTES];
    AHCI_READ_DMA_EXT(selectedStorageDevice->Port, Start_LBA, 1, buffer);

    BPB* bpb = (BPB*)buffer;

    context.FatStartLBA = Start_LBA + bpb->ReservedSectors;
    context.DataRegionStartLBA = context.FatStartLBA + (bpb->AmountOfFAT_Copies * bpb->SectorsPerFAT);
    context.SectorsPerCluster = bpb->SectorsPerCluster;
    context.RootCluster = bpb->RootBegin_ClusterNumber;
    context.BytesPerSector = bpb->BytesPerSector;
    context.RootDirLBA = context.DataRegionStartLBA + (bpb->RootBegin_ClusterNumber - 2) * bpb->SectorsPerCluster;
    context.ClusterSizeInBytes = context.SectorsPerCluster * context.BytesPerSector;

    return true;
}
#pragma endregion INIT


FilesystemDriver FAT32_Driver = {
    .init = init_fat32,
    .find_entry = find_entry,
    .get_entries = get_entries,
    .read = fat32_read,
    .write = fat32_write
};