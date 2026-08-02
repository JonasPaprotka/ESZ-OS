#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "memory.h"
#include "string.h"

TEST_CASE("allocated memory survives allocations") {
    char* a = (char*) malloc(16);
    str_copy(a, "ESZ-OS");
    malloc(4096);
    CHECK(str_equal(a, "ESZ-OS"));
}

TEST_CASE("free makes space reusable") {
    void* a = malloc(1024);
    free(a);
    void* b = malloc(1024);
    CHECK(b != nullptr);
}

TEST_CASE("allocs and frees dont fragment the heap") {
    for (int i = 0; i < 10000; i++) {
        void* p = malloc(128);
        free(p);
    }
    CHECK(malloc(1024) != nullptr);
}

TEST_CASE("malloc returns non overlapping regions") {
    void* a = malloc(64);
    void* b = malloc(64);

    const bool noOverlap = (uintptr_t)b >= (uintptr_t)a + 64 or (uintptr_t)a >= (uintptr_t)b + 64;
    CHECK(noOverlap);
}
