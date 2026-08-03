#include "memory.h"

void memory_fill(void* target, const uint8_t value, const uint64_t n) {
    // fill 8 bytes using uint64_t to make it faster
    uint64_t* dest64 = (uint64_t*) target;
    const uint64_t count64 = n / 8;

    const uint64_t value64 = (uint64_t) value * 0x0101010101010101ULL; //[AI-Supported Code] clone 1 byte to 8

    for (uint64_t i = 0; i < count64; ++i) {
        dest64[i] = value64;
    }

    // remaining bytes if not divideable by 8
    uint8_t* dest8 = (uint8_t*) (dest64 + count64);

    for (uint64_t i = 0; i < (n % 8); ++i) {
        dest8[i] = value;
    }
}

void memory_clear(void* target, const uint64_t n) {
    memory_fill(target, 0, n);
}

void memory_copy(void* copyTo, const void* copyFrom, const uint64_t n) {
    // fill 8 bytes using uint64_t to make it faster
    uint64_t* dest64 = (uint64_t*) copyTo;
    const uint64_t* src64 = (const uint64_t*) copyFrom;
    const uint64_t count64 = n / 8;

    for (uint64_t i = 0; i < count64; ++i) {
        dest64[i] = src64[i];
    }

    // remaining bytes if not divideable by 8
    unsigned char* dest8 = (unsigned char*) (dest64 + count64);
    const unsigned char* src8 = (const unsigned char*) (src64 + count64);

    for (uint64_t i = 0; i < (n % 8); ++i) {
        dest8[i] = src8[i];
    }
}
