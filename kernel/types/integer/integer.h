#pragma once

#include <stdint.h>

uint64_t digit_value(const char c);
int64_t to_int(const char* text, const uint64_t base);
int64_t to_int(const char* text); // base 10
