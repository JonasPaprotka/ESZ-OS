#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "string.h"

TEST_CASE("str_length") {
    CHECK(str_length("") == 0);
    CHECK(str_length(" ") == 1);
    CHECK(str_length("ESZ-OS") == 6);
    CHECK(str_length("Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut l") == 100);
}

TEST_CASE("str_equal") {
    CHECK(str_equal("A", "A") == true);
    CHECK(str_equal("A", "B") == false);
    CHECK(str_equal("A", "a") == false);
    CHECK(str_equal("Lorem ipsum dolor", "Lorem ipsum dolor") == true);
    CHECK(str_equal(" ", " ") == true);
    CHECK(str_equal("", "") == true);
}
