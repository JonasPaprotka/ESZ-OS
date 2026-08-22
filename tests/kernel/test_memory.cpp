#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "memory.h"

static bool all_bytes_are(const uint8_t* buffer, const uint64_t n, const uint8_t value) {
    for (uint64_t i = 0; i < n; i++) {
        if (buffer[i] != value) return false;
    }
    return true;
}

TEST_CASE("memory_fill") {
    uint8_t buffer[32];

    SUBCASE("zero bytes writes nothing") {
        memory_fill(buffer, 0xAA, 32);
        memory_fill(buffer, 0xBB, 0);
        CHECK(all_bytes_are(buffer, 32, 0xAA) == true);
    }
    SUBCASE("less than one 64 bit block") {
        memory_fill(buffer, 0xAA, 32);
        memory_fill(buffer, 0xBB, 7);
        CHECK(all_bytes_are(buffer, 7, 0xBB) == true);
        CHECK(buffer[7] == 0xAA);
    }
    SUBCASE("exactly one 64 bit block") {
        memory_fill(buffer, 0xAA, 32);
        memory_fill(buffer, 0xBB, 8);
        CHECK(all_bytes_are(buffer, 8, 0xBB) == true);
        CHECK(buffer[8] == 0xAA);
    }
    SUBCASE("not divideable by 8 uses both paths") {
        memory_fill(buffer, 0xAA, 32);
        memory_fill(buffer, 0xBB, 13);
        CHECK(all_bytes_are(buffer, 13, 0xBB) == true);
        CHECK(buffer[13] == 0xAA);
    }
    SUBCASE("full buffer") {
        memory_fill(buffer, 0xBB, 32);
        CHECK(all_bytes_are(buffer, 32, 0xBB) == true);
    }
    SUBCASE("value 0") {
        memory_fill(buffer, 0xAA, 32);
        memory_fill(buffer, 0, 32);
        CHECK(all_bytes_are(buffer, 32, 0) == true);
    }
    SUBCASE("value 255") {
        memory_fill(buffer, 0xFF, 32);
        CHECK(all_bytes_are(buffer, 32, 0xFF) == true);
    }
}

TEST_CASE("memory_clear") {
    uint8_t buffer[32];

    SUBCASE("zero bytes writes nothing") {
        memory_fill(buffer, 0xAA, 32);
        memory_clear(buffer, 0);
        CHECK(all_bytes_are(buffer, 32, 0xAA) == true);
    }
    SUBCASE("not divideable by 8") {
        memory_fill(buffer, 0xAA, 32);
        memory_clear(buffer, 13);
        CHECK(all_bytes_are(buffer, 13, 0) == true);
        CHECK(buffer[13] == 0xAA);
    }
    SUBCASE("full buffer") {
        memory_fill(buffer, 0xAA, 32);
        memory_clear(buffer, 32);
        CHECK(all_bytes_are(buffer, 32, 0) == true);
    }
}

TEST_CASE("memory_copy") {
    uint8_t source[32];
    uint8_t target[32];

    for (uint64_t i = 0; i < 32; i++) source[i] = (uint8_t) (i + 1);

    SUBCASE("zero bytes writes nothing") {
        memory_fill(target, 0xAA, 32);
        memory_copy(target, source, 0);
        CHECK(all_bytes_are(target, 32, 0xAA) == true);
    }
    SUBCASE("less than one 64 bit block") {
        memory_fill(target, 0xAA, 32);
        memory_copy(target, source, 7);

        for (uint64_t i = 0; i < 7; i++) CHECK(target[i] == source[i]);
        CHECK(target[7] == 0xAA);
    }
    SUBCASE("exactly one 64 bit block") {
        memory_fill(target, 0xAA, 32);
        memory_copy(target, source, 8);

        for (uint64_t i = 0; i < 8; i++) CHECK(target[i] == source[i]);
        CHECK(target[8] == 0xAA);
    }
    SUBCASE("not divideable by 8 uses both paths") {
        memory_fill(target, 0xAA, 32);
        memory_copy(target, source, 13);

        for (uint64_t i = 0; i < 13; i++) CHECK(target[i] == source[i]);
        CHECK(target[13] == 0xAA);
    }
    SUBCASE("full buffer") {
        memory_fill(target, 0xAA, 32);
        memory_copy(target, source, 32);

        for (uint64_t i = 0; i < 32; i++) CHECK(target[i] == source[i]);
    }
    SUBCASE("source stays untouched") {
        memory_fill(target, 0xAA, 32);
        memory_copy(target, source, 32);

        for (uint64_t i = 0; i < 32; i++) CHECK(source[i] == (uint8_t) (i + 1));
    }
}
