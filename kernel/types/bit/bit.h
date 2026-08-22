#pragma once

#include <stdint.h>

void bit_set(unsigned char* bitmap, const uint64_t page);
void bit_clear(unsigned char* bitmap, const uint64_t page);

void bit_write(unsigned char* bitmap, const uint64_t page, const bool value);
bool bit_read(unsigned char* bitmap, const uint64_t page);
