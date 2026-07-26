#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "config.h"
#include <stdint.h>

struct Entry {
    char Name[256];
    uint32_t Size;
    bool IsDirectory;
    uint32_t FirstCluster;
    bool Found;
};

struct FilesystemDriver {
    bool (*init)(uint32_t startLBA);
    Entry (*find_entry)(const char* path);
    uint64_t (*get_entries)(const char* path, Entry*& outEntries);
    uint8_t* (*read)(const Entry& entry);
    uint64_t (*write)(const Entry& entry, const uint8_t* data, uint64_t size);
};

extern char currentPath[MAX_PATH_LENGTH + 1];

bool is_root_path(const char* path);

void filesystem_print_working_directory();
void filesystem_list();
void filesystem_change_directory(const char* path);
bool init_filesystem();

#endif // FILESYSTEM_H
