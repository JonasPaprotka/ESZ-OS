#include "filesystem.h"
#include "color.h"
#include "print.h"
#include "storage.h"
#include "heap.h"
#include "string.h"

char currentPath[MAX_PATH_LENGTH + 1];

#pragma region HELPER FUNCTIONS
bool is_root_path(const char* path) {
    return (path[0] == '/') && (path[1] == 0);
}

bool is_absolute_path(const char* path) {
    return path[0] == '/';
}
#pragma endregion HELPER FUNCTIONS

// pwd
void filesystem_print_working_directory() {
    print(currentPath);
}

// ls
void filesystem_list() {
    Entry* entries;
    const char* trimmedPath = str_cut_end_malloc(currentPath, 1);
    uint64_t count = activeDriver->get_entries(trimmedPath, entries);
    free(trimmedPath);

    if (count == 0) return;

    for (uint64_t i = 0; i < count; i++) {
        const Entry currEntry = entries[i];
        if (currEntry.IsDirectory) {
            print_inline(currEntry.Name, Color::Yellow);
            print_inline("/", Color::Yellow);
        } else {
            print_inline(currEntry.Name);
        }
        print_inline("  ");
    }

    newline();
    free(entries);
}

// cd
void filesystem_change_directory(const char* path) {
    if (path[0] == 0) {
        currentPath[0] = '/';
        currentPath[1] = 0;
        return;
    }

    if (is_absolute_path(path)) {
        //TODO check if exists

        str_copy(currentPath, path);
        return;
    }

    char* pathSections[MAX_PATH_DEPTH];
    uint64_t splitCount = 0;
    str_split(path, '/', pathSections, splitCount);

    if (splitCount == 1) {
        //TODO check if exists

        str_add(currentPath, String(pathSections[0], "/"));
        return;
    }

    //TODO
}

bool init_filesystem() {
    if (!init_storage()) return false;

    currentPath[0] = '/';
    currentPath[1] = 0;

    return true;
}
