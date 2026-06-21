#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdint.h>

struct FileEntry {
    char Name[64];
    uint32_t Size;
    bool IsDirectory;
    uint32_t FirstCluster;
};

#endif // FILESYSTEM_H
