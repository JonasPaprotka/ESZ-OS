#include <stdint.h>

uint64_t divideRoundUp1(const uint64_t a, const uint64_t b) {
    // round up to next page if needed
    return (a + b - 1) / b;
}
