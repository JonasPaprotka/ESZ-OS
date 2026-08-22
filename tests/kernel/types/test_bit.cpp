#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "bit.h"

TEST_CASE("bit_set") {
    unsigned char bitmap[] = { 0b01000010 };
    bit_set(bitmap, 0);
    bit_set(bitmap, 1);
    bit_set(bitmap, 2);
    CHECK(bitmap[0] == 0b01000111);

    bit_set(bitmap, 10);
    CHECK(bitmap[0] == 0b01000111);
}

TEST_CASE("bit_clear") {
    unsigned char bitmap[] = { 0b01001010 };
    bit_clear(bitmap, 0);
    bit_clear(bitmap, 1);
    bit_clear(bitmap, 2);
    CHECK(bitmap[0] == 0b01000000);

    bit_clear(bitmap, 10);
    CHECK(bitmap[0] == 0b01000000);
}

TEST_CASE("bit_write") {
    unsigned char bitmap[] = { 0b01001010 };
    bit_write(bitmap, 0, true);
    bit_write(bitmap, 1, true);
    bit_write(bitmap, 2, true);
    bit_write(bitmap, 3, false);
    bit_write(bitmap, 4, false);
    bit_write(bitmap, 5, false);
    bit_write(bitmap, 6, true);
    bit_write(bitmap, 7, true);
    CHECK(bitmap[0] == 0b11000111);
}

TEST_CASE("bit_read") {
    unsigned char bitmap[] = { 0b10001110 };
    CHECK(bit_read(bitmap, 0) == false);
    CHECK(bit_read(bitmap, 1) == true);
    CHECK(bit_read(bitmap, 2) == true);
    CHECK(bit_read(bitmap, 3) == true);
    CHECK(bit_read(bitmap, 4) == false);
    CHECK(bit_read(bitmap, 5) == false);
    CHECK(bit_read(bitmap, 6) == false);
    CHECK(bit_read(bitmap, 7) == true);
}
