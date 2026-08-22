#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "char.h"

TEST_CASE("is_digit") {
    for(uint64_t i = 0; i < 47; i++) {
        CHECK(is_digit(i) == false);
    }
    for(uint64_t i = 48; i <= 57; i++) {
        CHECK(is_digit(i) == true);
    }
    for(uint64_t i = 58; i < 127; i++) {
        CHECK(is_digit(i) == false);
    }
}

TEST_CASE("is_alpha") {
    for(uint64_t i = 0; i < 64; i++) {
        CHECK(is_alpha(i) == false);
    }
    for(uint64_t i = 65; i <= 90; i++) {
        CHECK(is_alpha(i) == true);
    }
    for(uint64_t i = 91; i < 96; i++) {
        CHECK(is_alpha(i) == false);
    }
    for(uint64_t i = 97; i <= 122; i++) {
        CHECK(is_alpha(i) == true);
    }
    for(uint64_t i = 123; i < 127; i++) {
        CHECK(is_alpha(i) == false);
    }
}

TEST_CASE("is_upper_alpha") {
    for(uint64_t i = 0; i < 64; i++) {
        CHECK(is_upper_alpha(i) == false);
    }
    for(uint64_t i = 65; i <= 90; i++) {
        CHECK(is_upper_alpha(i) == true);
    }
    for(uint64_t i = 91; i < 127; i++) {
        CHECK(is_upper_alpha(i) == false);
    }
}

TEST_CASE("is_lower_alpha") {
    for(uint64_t i = 0; i < 96; i++) {
        CHECK(is_lower_alpha(i) == false);
    }
    for(uint64_t i = 97; i <= 122; i++) {
        CHECK(is_lower_alpha(i) == true);
    }
    for(uint64_t i = 123; i < 127; i++) {
        CHECK(is_lower_alpha(i) == false);
    }
}

TEST_CASE("is_whitespace") {
    for(uint64_t i = 0; i < 8; i++) {
        CHECK(is_whitespace(i) == false);
    }
    CHECK(is_whitespace(9) == true);
    for(uint64_t i = 10; i < 31; i++) {
        CHECK(is_whitespace(i) == false);
    }
    CHECK(is_whitespace(32) == true);
    for(uint64_t i = 33; i < 127; i++) {
        CHECK(is_whitespace(i) == false);
    }
}

TEST_CASE("to_upper") {
    CHECK(to_upper('1') == '1');
    CHECK(to_upper('#') == '#');
    CHECK(to_upper('a') == 'A');
    CHECK(to_upper('A') == 'A');
    CHECK(to_upper('b') == 'B');
    CHECK(to_upper('B') == 'B');
    CHECK(to_upper(' ') == ' ');
    CHECK(to_upper(9) == 9);
}

TEST_CASE("to_lower") {
    CHECK(to_lower('1') == '1');
    CHECK(to_lower('#') == '#');
    CHECK(to_lower('a') == 'a');
    CHECK(to_lower('A') == 'a');
    CHECK(to_lower('b') == 'b');
    CHECK(to_lower('B') == 'b');
    CHECK(to_lower(' ') == ' ');
    CHECK(to_lower(9) == 9);
}
