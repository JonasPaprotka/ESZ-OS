#ifndef MEMORY_INFO_H
#define MEMORY_INFO_H

#include <stdint.h>

void memory_info_init();
void print_memory_info();
void print_memory_fragmentation_graph(const uint64_t maxBlocks, const bool showSize);

#endif // MEMORY_INFO_H
