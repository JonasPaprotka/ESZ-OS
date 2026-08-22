#pragma once

#include <stdint.h>

void memory_info_init();
void print_memory_info();
void print_memory_fragmentation_graph(const uint64_t maxBlocks, const bool showSize);
