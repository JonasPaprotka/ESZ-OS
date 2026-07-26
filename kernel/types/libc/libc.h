#ifndef LIBC_H
#define LIBC_H

#include <stdint.h>

extern "C" void* memcpy(void* dest, const void* src, uint64_t n);
extern "C" void* memset(void* dest, int value, uint64_t n);
extern "C" void* memmove(void* dest, const void* src, uint64_t n);

#endif // LIBC_H
