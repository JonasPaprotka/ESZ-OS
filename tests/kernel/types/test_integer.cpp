#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "integer.h"
#include "char.h"
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

TEST_CASE("digit_value - digits") {
    for(uint64_t i = 48; i <= 57; i++) {
        CHECK(digit_value(i) == i - 48);
    }
    CHECK(digit_value('0') == 0);
    CHECK(digit_value('5') == 5);
    CHECK(digit_value('9') == 9);
}

TEST_CASE("digit_value - lower case letters") {
    for(uint64_t i = 97; i <= 122; i++) {
        CHECK(digit_value(i) == i - 97 + 10);
    }
    CHECK(digit_value('a') == 10);
    CHECK(digit_value('f') == 15);
    CHECK(digit_value('z') == 35);
}

TEST_CASE("digit_value - upper case letters") {
    for(uint64_t i = 65; i <= 90; i++) {
        CHECK(digit_value(i) == i - 65 + 10);
    }
    CHECK(digit_value('A') == 10);
    CHECK(digit_value('F') == 15);
    CHECK(digit_value('Z') == 35);
}

TEST_CASE("digit_value - case mismatch") {
    for(uint64_t i = 97; i <= 122; i++) {
        CHECK(digit_value(i) == digit_value(to_upper(i)));
    }
    CHECK(digit_value('a') == digit_value('A'));
    CHECK(digit_value('z') == digit_value('Z'));
}

TEST_CASE("digit_value - invalid chars") {
    for(uint64_t i = 0; i < 48; i++) {
        CHECK(digit_value(i) == 0);
    }
    for(uint64_t i = 58; i < 65; i++) {
        CHECK(digit_value(i) == 0);
    }
    for(uint64_t i = 91; i < 97; i++) {
        CHECK(digit_value(i) == 0);
    }
    for(uint64_t i = 123; i < 127; i++) {
        CHECK(digit_value(i) == 0);
    }
}

TEST_CASE("digit_value - round trip with to_string") {
    for(uint64_t i = 1; i <= 35; i++) {
        CHECK(str_length(to_string(i, 36)) == 1);
        CHECK(digit_value(to_string(i, 36)[0]) == i);
    }
}

TEST_CASE("test is_integer char") {
    CHECK(is_integer(' ') == false);
    for(uint64_t i = 0; i <= 9; i++) {
        CHECK(is_integer(to_string(i)) == true);
    }
    CHECK(is_integer('-') == false);
    CHECK(is_integer('+') == false);
}

TEST_CASE("test is_integer text") {
    CHECK(is_integer("") == false);
    CHECK(is_integer(" ") == false);
    CHECK(is_integer(" 1") == false);
    CHECK(is_integer(" p") == false);
    CHECK(is_integer(" P") == false);
    CHECK(is_integer("P") == false);
    CHECK(is_integer("P ") == false);
    CHECK(is_integer("p ") == false);
    CHECK(is_integer(" p ") == false);
    CHECK(is_integer(" p ") == false);
    CHECK(is_integer("this is a Test") == false);
    CHECK(is_integer("!") == false);
    CHECK(is_integer("&") == false);
    CHECK(is_integer("-") == false);
    CHECK(is_integer("+") == false);
    CHECK(is_integer("0") == true);
    CHECK(is_integer("010") == true);
    CHECK(is_integer("-100") == true);
    CHECK(is_integer("+100") == true);
    CHECK(is_integer("p100") == false);
    CHECK(is_integer("100p") == false);
    CHECK(is_integer("978423") == true);
    CHECK(is_integer(to_string(UINT64_MAX)) == true);
    CHECK(is_integer(to_string(INT64_MAX)) == true);
    CHECK(is_integer(to_string(INT64_MIN)) == true);
    CHECK(is_integer(to_string(INT16_MAX)) == true);
    CHECK(is_integer(to_string(INT16_MIN)) == true);
    CHECK(is_integer(to_string(INT32_MAX)) == true);
    CHECK(is_integer(to_string(INT32_MIN)) == true);
}
