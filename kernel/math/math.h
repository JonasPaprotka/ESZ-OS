#ifndef MATH_H
#define MATH_H

#include <stdint.h>

int64_t divide_round_up(const int64_t a, const int64_t b);
int64_t min(const int64_t a, const int64_t b);
int64_t max(const int64_t a, const int64_t b);

uint64_t abs(const int64_t n);
int64_t clamp(const int64_t n, const int64_t min, const int64_t max);
bool is_power_of_two(const int64_t n);

bool is_aligned(const uint64_t addr, const uint64_t align);
uint64_t align_up(const uint64_t addr, const uint64_t align);
uint64_t align_down(const uint64_t addr, const uint64_t align);

#endif // MATH_H
