#include <stdint.h>

int64_t divide_round_up(const int64_t a, const int64_t b) {
    return (a + b - 1) / b;
}

int64_t min(const int64_t a, const int64_t b) {
    if (a <= b) return a;
    return b;
}

int64_t max(const int64_t a, const int64_t b) {
    if (a >= b) return a;
    return b;
}

uint64_t abs(const int64_t n) {
    if (n >= 0) return n;
    return -(n);
}

int64_t clamp(const int64_t n, const int64_t min, const int64_t max) {
    if (n <= min) return min;
    if (n >= max) return max;
    return n;
}

bool is_power_of_two(const int64_t n) {
    return n > 0 && (n & (n - 1)) == 0;
}

bool is_aligned(const uint64_t addr, const uint64_t align) {
    return addr % align == 0;
}

// e.g. 5000, 4096 -> 8192
uint64_t align_up(const uint64_t addr, const uint64_t align) {
    if (is_aligned(addr, align)) return addr;
    return (addr + align) - (addr & align);
}

// e.g. 5000, 4096 -> 4096
uint64_t align_down(const uint64_t addr, const uint64_t align) {
    return addr - (addr % align);
}
