#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "math.h"

TEST_CASE("min") {
    CHECK(min(0, 0) == 0);
    CHECK(min(1, 0) == 0);
    CHECK(min(0, 1) == 0);
    CHECK(min(99, 11) == 11);
    CHECK(min(5, 6) == 5);
}

TEST_CASE("max") {
    CHECK(max(0, 0) == 0);
    CHECK(max(1, 0) == 1);
    CHECK(max(0, 1) == 1);
    CHECK(max(99, 11) == 99);
    CHECK(max(5, 6) == 6);
}

TEST_CASE("divide_round_up") {
    CHECK(divide_round_up(0, 0) == 0);
    CHECK(divide_round_up(0, 1) == 0);
    CHECK(divide_round_up(1, 0) == 0);

    CHECK(divide_round_up(8, 2) == 4);
    CHECK(divide_round_up(16, 4) == 4);
    CHECK(divide_round_up(5, 5) == 1);

    CHECK(divide_round_up(3000, 4096) == 1);
    CHECK(divide_round_up(5000, 4096) == 2);
}

TEST_CASE("abs_value") {
    CHECK(abs_value(0) == 0);
    CHECK(abs_value(1) == 1);
    CHECK(abs_value(100) == 100);
    CHECK(abs_value(INT64_MAX) == 9223372036854775807ULL);
    CHECK(abs_value(INT64_MIN) == 9223372036854775808ULL);
    CHECK(abs_value(-100) == 100);
    CHECK(abs_value(-1) == 1);
    CHECK(abs_value(-0) == 0);
}

TEST_CASE("is_power_of_two") {
    CHECK(is_power_of_two(0) == false);
    CHECK(is_power_of_two(1) == true);
    CHECK(is_power_of_two(2) == true);
    CHECK(is_power_of_two(3) == false);
    CHECK(is_power_of_two(4) == true);
    CHECK(is_power_of_two(5) == false);
    CHECK(is_power_of_two(6) == false);
    CHECK(is_power_of_two(7) == false);
    CHECK(is_power_of_two(8) == true);
    CHECK(is_power_of_two(9) == false);
    CHECK(is_power_of_two(10) == false);
    CHECK(is_power_of_two(11) == false);
    CHECK(is_power_of_two(12) == false);
    CHECK(is_power_of_two(13) == false);
    CHECK(is_power_of_two(14) == false);
    CHECK(is_power_of_two(15) == false);
    CHECK(is_power_of_two(16) == true);
    CHECK(is_power_of_two(17) == false);
}

TEST_CASE("is_aligned") {
    CHECK(is_aligned(0, 4096) == true);
    CHECK(is_aligned(0, 0) == true);
    CHECK(is_aligned(1, 0) == false);
    CHECK(is_aligned(0, 1) == true);
    CHECK(is_aligned(1, 1) == true);
    CHECK(is_aligned(100, 5) == true);
    CHECK(is_aligned(150, 50) == true);
    CHECK(is_aligned(15, 50) == false);
    CHECK(is_aligned(4096, 4096) == true);
    CHECK(is_aligned(8192, 4096) == true);
    CHECK(is_aligned(8191, 4096) == false);
    CHECK(is_aligned(8193, 4096) == false);
}

TEST_CASE("align_up") {
    CHECK(align_up(0, 0) == 0);
    CHECK(align_up(0, 1) == 0);
    CHECK(align_up(0, 10) == 0);
    CHECK(align_up(5, 10) == 10);
    CHECK(align_up(8191, 4096) == 8192);
    CHECK(align_up(5000, 4096) == 8192);
    CHECK(align_up(4000, 4096) == 4096);
}

TEST_CASE("align_down") {
    CHECK(align_down(0, 0) == 0);
    CHECK(align_down(0, 1) == 0);
    CHECK(align_down(2, 10) == 0);
    CHECK(align_down(5, 10) == 0);
    CHECK(align_down(8191, 4096) == 4096);
    CHECK(align_down(5000, 4096) == 4096);
    CHECK(align_down(4000, 4096) == 0);
}

TEST_CASE("clamp") {
    CHECK(clamp(0, 0, 0) == 0);
    CHECK(clamp(5, 0, 0) == 0);
    CHECK(clamp(5, 0, 5) == 5);
    CHECK(clamp(5, 5, 5) == 5);
    CHECK(clamp(5, 6, 6) == 6);
    CHECK(clamp(5, 6, 7) == 6);
    CHECK(clamp(8, 6, 7) == 7);
    CHECK(clamp(15, 10, 1) == 10);
    CHECK(clamp(15, 1, 10) == 10);
}
