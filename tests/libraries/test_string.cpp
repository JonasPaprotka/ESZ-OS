#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "string.h"

TEST_CASE("str_length") {
    CHECK(str_length("") == 0);
    CHECK(str_length(" ") == 1);
    CHECK(str_length("ESZ-OS") == 6);
    CHECK(str_length("Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut l") == 100);
}
