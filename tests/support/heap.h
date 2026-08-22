#pragma once

#include <cstdlib>

inline void free(const void* ptr) { ::free(const_cast<void*>(ptr)); }
