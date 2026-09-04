#pragma once

#include <stdint.h>

bool as_bool(const uint64_t value);
bool as_bool(const char c);
bool as_bool(const char* text);

bool is_bool(const uint64_t value);
bool is_bool(const char c);
bool is_bool(const char* text);
