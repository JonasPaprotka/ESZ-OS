#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "boolean.h"

TEST_CASE("as_bool uint64_t") {
    CHECK(as_bool((uint64_t) 0) == false);
    CHECK(as_bool((uint64_t) 1) == true);
    for(uint64_t i = 2; i < 16; i++) {
        CHECK(as_bool((uint64_t) i) == false);
    }
}

TEST_CASE("as_bool char") {
    CHECK(as_bool('t') == true);
    CHECK(as_bool('y') == true);
    CHECK(as_bool('f') == false);
    CHECK(as_bool('n') == false);
    CHECK(as_bool('1') == true);
    CHECK(as_bool('0') == false);
    CHECK(as_bool('+') == false);
    CHECK(as_bool('-') == false);
    CHECK(as_bool('p') == false);
    CHECK(as_bool('Y') == true);
    CHECK(as_bool('T') == true);
    CHECK(as_bool('F') == false);
    CHECK(as_bool('N') == false);
}

TEST_CASE("as_bool text") {
    CHECK(as_bool("") == false);
    CHECK(as_bool("test") == false);
    CHECK(as_bool("True") == true);
    CHECK(as_bool("TRUE") == true);
    CHECK(as_bool("true") == true);
    CHECK(as_bool("Yes") == true);
    CHECK(as_bool("yes") == true);
    CHECK(as_bool("YES") == true);
    CHECK(as_bool("FALSE") == false);
    CHECK(as_bool("False") == false);
    CHECK(as_bool("false") == false);
    CHECK(as_bool("No") == false);
    CHECK(as_bool("NO") == false);
    CHECK(as_bool("no") == false);
    CHECK(as_bool("!&%") == false);
    CHECK(as_bool(" ") == false);
    CHECK(as_bool("test test") == false);
    CHECK(as_bool("1") == true);
    CHECK(as_bool("0") == false);
    CHECK(as_bool("2") == false);
    CHECK(as_bool("3") == false);
    CHECK(as_bool("4") == false);
    CHECK(as_bool("15") == false);
}

TEST_CASE("is_bool uint64_t") {
    CHECK(is_bool((uint64_t) 0) == true);
    CHECK(is_bool((uint64_t) 1) == true);
    for(uint64_t i = 2; i < 16; i++) {
        CHECK(is_bool((uint64_t) i) == false);
    }
}

TEST_CASE("is_bool char") {
    CHECK(is_bool(' ') == false);
    CHECK(is_bool('a') == false);
    CHECK(is_bool('A') == false);
    CHECK(is_bool('1') == true);
    CHECK(is_bool('0') == true);
    CHECK(is_bool('3') == false);
    CHECK(is_bool('2') == false);
    CHECK(is_bool('!') == false);
    CHECK(is_bool('y') == true);
    CHECK(is_bool('Y') == true);
    CHECK(is_bool('t') == true);
    CHECK(is_bool('T') == true);
    CHECK(is_bool('f') == true);
    CHECK(is_bool('F') == true);
    CHECK(is_bool('n') == true);
    CHECK(is_bool('N') == true);
}

TEST_CASE("is_bool text") {
    CHECK(is_bool("") == false);
    CHECK(is_bool(" ") == false);
    CHECK(is_bool(" y") == false);
    CHECK(is_bool(" a") == false);
    CHECK(is_bool("A") == false);
    CHECK(is_bool("!") == false);
    CHECK(is_bool("2") == false);
    CHECK(is_bool("123") == false);
    CHECK(is_bool("YES!") == false);
    CHECK(is_bool("YES") == true);
    CHECK(is_bool("yes") == true);
    CHECK(is_bool("true") == true);
    CHECK(is_bool("TRUE") == true);
    CHECK(is_bool("false") == true);
    CHECK(is_bool("FALSE") == true);
    CHECK(is_bool("no") == true);
    CHECK(is_bool("NO") == true);
    CHECK(is_bool("1") == true);
    CHECK(is_bool("0") == true);
}
