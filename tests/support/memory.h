#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <cstdlib>
#include <cstring>

inline void free(const void* ptr) { ::free((void*) ptr); }
inline void memory_clear(void* target, uint64_t n) { ::memset(target, 0, n); }

#endif // MEMORY_H
