#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "string.h"

TEST_CASE("str_length") {
    SUBCASE("empty") {
        CHECK(str_length("") == 0);
    }
    SUBCASE("single space") {
        CHECK(str_length(" ") == 1);
    }
    SUBCASE("word") {
        CHECK(str_length("ESZ-OS") == 6);
    }
    SUBCASE("long sentence") {
        CHECK(str_length("Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut l") == 100);
    }
}

TEST_CASE("str_equal") {
    SUBCASE("equal single char") {
        CHECK(str_equal("A", "A") == true);
    }
    SUBCASE("different length") {
        CHECK(str_equal("1", "12") == false);
    }
    SUBCASE("different char") {
        CHECK(str_equal("A", "B") == false);
    }
    SUBCASE("case mismatch") {
        CHECK(str_equal("A", "a") == false);
    }
    SUBCASE("equal sentence") {
        CHECK(str_equal("Lorem ipsum dolor", "Lorem ipsum dolor") == true);
    }
    SUBCASE("equal space") {
        CHECK(str_equal(" ", " ") == true);
    }
    SUBCASE("equal empty") {
        CHECK(str_equal("", "") == true);
    }
}

TEST_CASE("str_copy") {
    char dest[32];

    SUBCASE("single char") {
        str_copy(dest, "A");
        CHECK(str_equal(dest, "A"));
    }

    SUBCASE("multiple chars") {
        str_copy(dest, "ABC");
        CHECK(str_equal(dest, "ABC"));
    }

    SUBCASE("empty") {
        str_copy(dest, "");
        CHECK(str_equal(dest, ""));
    }

    SUBCASE("space") {
        str_copy(dest, " ");
        CHECK(str_equal(dest, " "));
    }

    SUBCASE("sentence") {
        str_copy(dest, "Lorem ipsum dolor");
        CHECK(str_equal(dest, "Lorem ipsum dolor"));
    }
}

TEST_CASE("str_add") {
    char dest[64];

    SUBCASE("empty target, string add") {
        dest[0] = 0;
        str_add(dest, "A");
        CHECK(str_equal(dest, "A"));
    }

    SUBCASE("via str_copy - multiple chars") {
        dest[0] = 0;
        str_copy(dest, "ABC");
        CHECK(str_equal(dest, "ABC"));
    }

    SUBCASE("via str_copy - empty") {
        dest[0] = 0;
        str_copy(dest, "");
        CHECK(str_equal(dest, ""));
    }

    SUBCASE("via str_copy - space") {
        dest[0] = 0;
        str_copy(dest, " ");
        CHECK(str_equal(dest, " "));
    }

    SUBCASE("via str_copy - sentence") {
        dest[0] = 0;
        str_copy(dest, "Lorem ipsum dolor");
        CHECK(str_equal(dest, "Lorem ipsum dolor"));
    }

    // --- FILLED TEST ---
    SUBCASE("filled target - append char") {
        str_copy(dest, "ABC ");
        str_add(dest, "A");
        CHECK(str_equal(dest, "ABC A"));
    }

    SUBCASE("filled target - append multiple") {
        str_copy(dest, "ABC ");
        str_add(dest, "ABC");
        CHECK(str_equal(dest, "ABC ABC"));
    }

    SUBCASE("filled target - append empty") {
        str_copy(dest, "ABC ");
        str_add(dest, "");
        CHECK(str_equal(dest, "ABC "));
    }

    SUBCASE("filled target - append space") {
        str_copy(dest, "ABC ");
        str_add(dest, " ");
        CHECK(str_equal(dest, "ABC  "));
    }

    SUBCASE("filled target - append sentence") {
        str_copy(dest, "ABC ");
        str_add(dest, "Lorem ipsum dolor");
        CHECK(str_equal(dest, "ABC Lorem ipsum dolor"));
    }
}

TEST_CASE("str_add (char param)") {
    char dest[64];

    SUBCASE("empty target - append char") {
        dest[0] = 0;
        str_add(dest, 'A');
        CHECK(str_equal(dest, "A"));
    }

    SUBCASE("empty target - append space") {
        dest[0] = 0;
        str_add(dest, ' ');
        CHECK(str_equal(dest, " "));
    }

    // --- FILLED TEST ---
    SUBCASE("filled target - append char") {
        str_copy(dest, "ABC ");
        str_add(dest, 'A');
        CHECK(str_equal(dest, "ABC A"));
    }

    SUBCASE("filled target - append space") {
        str_copy(dest, "ABC ");
        str_add(dest, ' ');
        CHECK(str_equal(dest, "ABC  "));
    }
}

TEST_CASE("str_contains") {
    char text[64];

    SUBCASE("empty text, empty search") {
        text[0] = 0;
        CHECK(str_contains(text, "") == true);
    }

    SUBCASE("empty text, non-empty search") {
        text[0] = 0;
        CHECK(str_contains(text, "TEXT") == false);
    }

    SUBCASE("sentence, empty search") {
        str_copy(text, "Lorem ipsum dolor sit amet");
        CHECK(str_contains(text, "") == true);
    }

    SUBCASE("sentence, not found") {
        str_copy(text, "Lorem ipsum dolor sit amet");
        CHECK(str_contains(text, "TEXT") == false);
    }

    SUBCASE("sentence, space") {
        str_copy(text, "Lorem ipsum dolor sit amet");
        CHECK(str_contains(text, " ") == true);
    }

    SUBCASE("sentence, leading word") {
        str_copy(text, "Lorem ipsum dolor sit amet");
        CHECK(str_contains(text, "Lorem") == true);
    }

    SUBCASE("sentence, mid word") {
        str_copy(text, "Lorem ipsum dolor sit amet");
        CHECK(str_contains(text, "ipsum") == true);
    }

    SUBCASE("sentence, case mismatch") {
        str_copy(text, "Lorem ipsum dolor sit amet");
        CHECK(str_contains(text, "IPSUM") == false);
    }

    SUBCASE("sentence, phrase") {
        str_copy(text, "Lorem ipsum dolor sit amet");
        CHECK(str_contains(text, "sit amet") == true);
    }

    SUBCASE("sentence, full match") {
        str_copy(text, "Lorem ipsum dolor sit amet");
        CHECK(str_contains(text, "Lorem ipsum dolor sit amet") == true);
    }
}

TEST_CASE("str_count") {
    char text[64];

    SUBCASE("empty text, empty search") {
        text[0] = 0;
        CHECK(str_count(text, "") == 1);
    }

    SUBCASE("sentence, empty search") {
        str_copy(text, "Lorem ipsum dolor sit amet");
        CHECK(str_count(text, "") == 27);
    }

    SUBCASE("sentence, space") {
        str_copy(text, "Lorem ipsum dolor sit amet");
        CHECK(str_count(text, " ") == 4);
    }

    SUBCASE("sentence, leading word") {
        str_copy(text, "Lorem ipsum dolor sit amet");
        CHECK(str_count(text, "Lorem") == 1);
    }

    SUBCASE("sentence, mid word") {
        str_copy(text, "Lorem ipsum dolor sit amet");
        CHECK(str_count(text, "ipsum") == 1);
    }

    SUBCASE("sentence, single char upper") {
        str_copy(text, "Lorem ipsum dolor sit amet");
        CHECK(str_count(text, "L") == 1);
    }

    SUBCASE("sentence, single char lower") {
        str_copy(text, "Lorem ipsum dolor sit amet");
        CHECK(str_count(text, "l") == 1);
    }

    SUBCASE("sentence, phrase") {
        str_copy(text, "Lorem ipsum dolor sit amet");
        CHECK(str_count(text, "sit amet") == 1);
    }

    SUBCASE("search text longer then text") {
        str_copy(text, "Lorem");
        CHECK(str_count(text, "Lorem ipsum") == 0);
    }
}

TEST_CASE("str_starts_with") {
    char text[64];

    SUBCASE("empty text") {
        text[0] = 0;
        CHECK(str_starts_with(text, "") == true);
    }

    SUBCASE("search = full text") {
        str_copy(text, "Search ME");
        CHECK(str_starts_with(text, "Search ME") == true);
    }

    SUBCASE("search = partial text") {
        str_copy(text, "Search ME");
        CHECK(str_starts_with(text, "Search") == true);
        CHECK(str_starts_with(text, "ME") == false);
    }

    SUBCASE("capitalisation check") {
        str_copy(text, "Search ME");
        CHECK(str_starts_with(text, "Search") == true);
        CHECK(str_starts_with(text, "search") == false);
        CHECK(str_starts_with(text, "Search ME") == true);
        CHECK(str_starts_with(text, "search me") == false);
    }

    SUBCASE("search text longer then text length") {
        str_copy(text, "Test");
        CHECK(str_starts_with(text, "ExampleTest") == false);
    }
}

TEST_CASE("str_ends_with") {
    char text[64];

    SUBCASE("empty text") {
        text[0] = 0;
        CHECK(str_ends_with(text, "") == true);
    }

    SUBCASE("search = full text") {
        str_copy(text, "Search ME");
        CHECK(str_ends_with(text, "Search ME") == true);
    }

    SUBCASE("search = partial text") {
        str_copy(text, "Search ME");
        CHECK(str_ends_with(text, "ME") == true);
        CHECK(str_ends_with(text, "Search") == false);
    }

    SUBCASE("capitalisation check") {
        str_copy(text, "Search ME");
        CHECK(str_ends_with(text, "ME") == true);
        CHECK(str_ends_with(text, "me") == false);
        CHECK(str_ends_with(text, "Search ME") == true);
        CHECK(str_ends_with(text, "search me") == false);
    }

    SUBCASE("search text longer then text length") {
        str_copy(text, "Test");
        CHECK(str_ends_with(text, "ExampleTest") == false);
    }
}

TEST_CASE("str_replace") {
    char text[128];

    SUBCASE("empty string") {
        text[0] = 0;
        str_replace(text, "", "REPLACED");
        CHECK(str_equal(text, "REPLACED"));
    }

    SUBCASE("longer replace text") {
        str_copy(text, "Lorem ipsum");
        str_replace(text, "Lorem ipsum sit", "REPLACED");
        CHECK(str_equal(text, "Lorem ipsum"));
    }

    SUBCASE("same length") {
        str_copy(text, "Lorem ipsum dolor sit amet.");
        str_replace(text, "dolor", "IPSUM");
        CHECK(str_equal(text, "Lorem ipsum IPSUM sit amet."));
    }

    SUBCASE("normal case") {
        str_copy(text, "Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet.");
        str_replace(text, "sit amet", "Replaced BY ME");
        CHECK(str_equal(text, "Lorem ipsum dolor Replaced BY ME. Lorem ipsum dolor Replaced BY ME."));
    }

    SUBCASE("capitalisation") {
        str_copy(text, "Lorem ipsum dolor sit amet.");
        str_replace(text, "Lorem", "REPLACED");
        CHECK(str_equal(text, "REPLACED ipsum dolor sit amet."));

        str_copy(text, "Lorem ipsum dolor sit amet.");
        str_replace(text, "lorem", "REPLACED");
        CHECK(str_equal(text, "Lorem ipsum dolor sit amet."));

        str_copy(text, "lorem ipsum dolor sit amet.");
        str_replace(text, "Lorem", "REPLACED");
        CHECK(str_equal(text, "lorem ipsum dolor sit amet."));
    }
}

TEST_CASE("str_repeat") {
    CHECK(str_equal(str_repeat("-", 10), "----------") == true);
    CHECK(str_equal(str_repeat("-#", 10), "-#-#-#-#-#-#-#-#-#-#") == true);
    CHECK(str_equal(str_repeat(" ", 10), "          ") == true);
    CHECK(str_equal(str_repeat("A", 1), "A") == true);
    CHECK(str_equal(str_repeat("A", 0), "") == true);
}

TEST_CASE("str_to_lower") {
    CHECK(str_equal(str_to_lower("abc"), "abc") == true);
    CHECK(str_equal(str_to_lower("ABC"), "abc") == true);
    CHECK(str_equal(str_to_lower("ABC abc"), "abc abc") == true);
    CHECK(str_equal(str_to_lower("Abc"), "abc") == true);
    CHECK(str_equal(str_to_lower("123A!#_B?0C"), "123a!#_b?0c") == true);
    CHECK(str_equal(str_to_lower(" "), " ") == true);
    CHECK(str_equal(str_to_lower(""), "") == true);
}

TEST_CASE("str_to_upper") {
    CHECK(str_equal(str_to_upper("abc"), "ABC") == true);
    CHECK(str_equal(str_to_upper("ABC"), "ABC") == true);
    CHECK(str_equal(str_to_upper("ABC abc"), "ABC ABC") == true);
    CHECK(str_equal(str_to_upper("Abc"), "ABC") == true);
    CHECK(str_equal(str_to_upper("123a!#_b?0c"), "123A!#_B?0C") == true);
    CHECK(str_equal(str_to_upper(" "), " ") == true);
    CHECK(str_equal(str_to_upper(""), "") == true);
}
