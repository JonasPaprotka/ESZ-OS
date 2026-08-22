#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "utf16.h"
#include "string.h"

TEST_CASE("utf16_char_to_ascii") {
    CHECK(utf16_char_to_ascii("A") == 'A');
    CHECK(utf16_char_to_ascii("z") == 'z');
    CHECK(utf16_char_to_ascii("5") == '5');
    CHECK(utf16_char_to_ascii(" ") == ' ');
    CHECK(utf16_char_to_ascii("") == 0);
}

TEST_CASE("utf16_text_to_ascii") {
    CHECK(str_equal(utf16_text_to_ascii("", 0), "") == true);
    CHECK(str_equal(utf16_text_to_ascii("A", 1), "A") == true);

    CHECK(str_equal(utf16_text_to_ascii("E\0S\0Z\0-\0O\0S\0", 6), "ESZ-OS") == true);
    CHECK(str_equal(utf16_text_to_ascii("E\0S\0Z\0-\0O\0S\0", 3), "ESZ") == true);

    CHECK(str_equal(utf16_text_to_ascii("A\0B\0\0\0C\0", 4), "AB") == true);
    CHECK(str_equal(utf16_text_to_ascii("A\0B\0\xFF\xFF", 4), "AB") == true);
}
