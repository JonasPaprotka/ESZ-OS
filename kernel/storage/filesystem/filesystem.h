#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdint.h>

struct Entry {
    char Name[64];
    uint32_t Size;
    bool IsDirectory;
    uint32_t FirstCluster;
    bool Found;
};

#endif // FILESYSTEM_H
