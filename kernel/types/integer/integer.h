#ifndef INTEGER_H
#define INTEGER_H

#include <stdint.h>

int64_t to_int(const char* text, const uint64_t base);
int64_t to_int(const char* text); // base 10

#endif //INTEGER_H
