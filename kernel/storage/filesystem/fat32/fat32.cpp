#include "fat32.h"
#include <stdint.h>
#include "info_text.h"
#include "ahci.h"
#include "print.h"
#include "string.h"
#include "utf16.h"
#include "filesystem.h"

bool isLFN(FAT_Attributes_Bits attributes) {
    return attributes.ReadOnly && attributes.Hidden && attributes.System && attributes.VolumeID;
}

void get_file_entries(Directory_Entry* entries, FileEntry outEntries[16], int& outEntryCount) {
    bool gotLongName = false;

    for (int i = 0; i < 16; i++) {
        if (entries[i].Name[0] == 0) break;

        const bool isEntryLFN = isLFN(entries[i].Attributes);

        if (isEntryLFN) {
            LFN_Entry_Struct* lfn = (LFN_Entry_Struct*)&entries[i];

            // BULD NAME
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
                char newName[12];

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

        if (!isEntryLFN) {
            outEntryCount++;
        }
    }
}

void init_fat32(const uint32_t Start_LBA) {
    uint8_t buffer[SECTOR_SIZE_BYTES];
    AHCI_READ_DMA_EXT(mainMassStorageDevice, Start_LBA, 1, buffer);

    BPB* bpb = (BPB*)buffer;

    const uint64_t FatStart_LBA = Start_LBA + bpb->ReservedSectors;
    const uint64_t DataRegionStart_LBA = FatStart_LBA + (bpb->AmountOfFAT_Copies * bpb->SectorsPerFAT);
    const uint64_t RootDir_LBA = DataRegionStart_LBA + (bpb->RootBegin_ClusterNumber - 2) * bpb->SectorsPerCluster;

    uint8_t rootDirBuffer[SECTOR_SIZE_BYTES];
    AHCI_READ_DMA_EXT(mainMassStorageDevice, RootDir_LBA, 1, rootDirBuffer);
    Directory_Entry* entries = (Directory_Entry*)rootDirBuffer;


    // ----- PARSE ------
    FileEntry fileEntries[16];
    memory_clear(fileEntries, sizeof(fileEntries));
    int fileEntryAmount = 0;
    get_file_entries(entries, fileEntries, fileEntryAmount);

    for (int i = 0; i < fileEntryAmount; i++) {
        if (fileEntries[i].IsDirectory) {
            print(String(str_to_lower(fileEntries[i].Name), "/"), Color::Blue);
        } else {
            print(String(str_to_lower(fileEntries[i].Name)), Color::Yellow);
        }
    }
}
