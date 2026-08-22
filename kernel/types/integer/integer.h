#pragma once

#include <stdint.h>

int64_t to_int(const char* text, const uint64_t base);
int64_t to_int(const char* text); // base 10
