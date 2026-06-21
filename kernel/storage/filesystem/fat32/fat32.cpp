#include "fat32.h"
#include <stdint.h>
#include "info_text.h"

void init_fat32(const uint32_t Start_LBA) {
    printInfoLine(InfoTextType::Debug, "INIT FAT32");

}
