#ifndef HEAP_H
#define HEAP_H

#include <cstdlib>

inline void free(const void* ptr) { ::free(const_cast<void*>(ptr)); }

#endif // HEAP_H
