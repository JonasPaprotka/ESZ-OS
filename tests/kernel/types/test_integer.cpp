#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "integer.h"
#include "string.h"

TEST_CASE("to_int - base 10") {
    CHECK(to_int("0") == 0);
    CHECK(to_int("1") == 1);
    CHECK(to_int("9") == 9);
    CHECK(to_int("10") == 10);
    CHECK(to_int("123") == 123);
    CHECK(to_int("999999") == 999999);
    CHECK(to_int("") == 0);
}

TEST_CASE("to_int - negative") {
    CHECK(to_int("-1") == -1);
    CHECK(to_int("-9") == -9);
    CHECK(to_int("-123") == -123);
    CHECK(to_int("-0") == 0);
}

TEST_CASE("to_int - base 2") {
    CHECK(to_int("0", 2) == 0);
    CHECK(to_int("1", 2) == 1);
    CHECK(to_int("10", 2) == 2);
    CHECK(to_int("1010", 2) == 10);
    CHECK(to_int("11111111", 2) == 255);
}

TEST_CASE("to_int - base 8") {
    CHECK(to_int("7", 8) == 7);
    CHECK(to_int("10", 8) == 8);
    CHECK(to_int("777", 8) == 511);
}

TEST_CASE("to_int - base 16") {
    CHECK(to_int("0", 16) == 0);
    CHECK(to_int("9", 16) == 9);
    CHECK(to_int("a", 16) == 10);
    CHECK(to_int("f", 16) == 15);
    CHECK(to_int("10", 16) == 16);
    CHECK(to_int("ff", 16) == 255);
    CHECK(to_int("1000", 16) == 4096);
    CHECK(to_int("-ff", 16) == -255);
}

TEST_CASE("to_int - base 36") {
    CHECK(to_int("9", 36) == 9);
    CHECK(to_int("a", 36) == 10);
    CHECK(to_int("z", 36) == 35);
    CHECK(to_int("10", 36) == 36);
}

TEST_CASE("to_int - round trip with to_string") {
    CHECK(to_int(to_string((uint64_t) 12345, 10), 10) == 12345);
    CHECK(to_int(to_string((uint64_t) 255, 16), 16) == 255);
    CHECK(to_int(to_string((uint64_t) 255, 2), 2) == 255);
    CHECK(to_int(to_string((uint64_t) 4096, 8), 8) == 4096);
    CHECK(to_int(to_string((uint64_t) 35, 36), 36) == 35);
}
