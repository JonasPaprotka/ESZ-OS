#ifndef PRINT_HELPER_H
#define PRINT_HELPER_H

#include <stdint.h>

void print_separator();
void printSysinfo();
void printTerminalHeader();
void printLoadingStart(const char* text);
void printLoadingStatus(const bool success);
void printPercentBar(const uint64_t count, const uint64_t outOf, const uint64_t elements, const bool showPercentage);

#endif // PRINT_HELPER_H
