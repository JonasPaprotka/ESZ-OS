#pragma once

#include <stdint.h>

inline uint64_t divide_round_up(const uint64_t a, const uint64_t b) {
    if (b == 0) return 0;
    return (a + b - 1) / b;
}

inline int64_t min(const int64_t a, const int64_t b) {
    if (a <= b) return a;
    return b;
}

inline int64_t max(const int64_t a, const int64_t b) {
    if (a >= b) return a;
    return b;
}

inline uint64_t abs_value(const int64_t n) {
    if (n >= 0) return n;
    return -(n);
}

inline int64_t clamp(const int64_t n, const int64_t min, const int64_t max) {
    if (max <= min) return min;
    if (n <= min) return min;
    if (n >= max) return max;
    return n;
}

inline bool is_power_of_two(const int64_t n) {
    return n > 0 && (n & (n - 1)) == 0;
}

inline bool is_aligned(const uint64_t addr, const uint64_t align) {
    if (align == 0) return addr == 0;
    return addr % align == 0;
}

// e.g. 5000, 4096 -> 8192
inline uint64_t align_up(const uint64_t addr, const uint64_t align) {
    if (align == 0) return addr;
    uint64_t remainder = addr % align;
    if (remainder == 0) return addr;
    return addr + (align - remainder);
}

// e.g. 5000, 4096 -> 4096
inline uint64_t align_down(const uint64_t addr, const uint64_t align) {
    if (align == 0) return addr;
    return addr - (addr % align);
}
