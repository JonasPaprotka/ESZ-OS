#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include "memory.h"

char* malloc_str(const uint64_t size);

int str_length(const char* str);
char* str_repeat(const char* text, int amount);
 
bool str_equal(const char* a, const char* b);

void str_copy(const char* src, const char* dest);
void str_add(char* target, const char* value);
char* str_combine(const char* a, const char* b);

bool str_contains(const char* text, const char* searchText);
int str_count(const char* text, const char* searchText);
bool str_starts_with(const char* text, const char* searchText);
bool str_ends_with(const char* text, const char* searchText);
void str_replace(char* text, const char* toBeReplacedText, const char* replacementText);


char* to_string(const uint64_t inputValue, const uint8_t basis);
char* to_string(const int64_t inputValue, const uint8_t basis);
char* to_string(const uint64_t inputValue);
char* to_string(const int64_t inputValue);
char* to_string(const int32_t inputValue);
char* to_string(const uint32_t inputValue);


struct String {
    char* data;
    
    static const char* to_cstr(const char* s) { return s; }
    static const char* to_cstr(uint64_t n) { return to_string(n); }
    static const char* to_cstr(int64_t n) { return to_string(n); }
    static const char* to_cstr(uint32_t n) { return to_string(n); }
    static const char* to_cstr(int32_t n) { return to_string(n); }

    // AI supported
    template<typename... Args>
    String(Args... args) {
        const char* parts[] = { to_cstr(args)... };
        data = malloc_str(1);

        for (uint64_t i = 0; i < sizeof...(args); i++) {
            char* combined = str_combine(data, parts[i]);
            free(data);
            data = combined;
        }
    }
    
    ~String() {
        free(data);
    }
    
    operator const char*() {
        return data;
    }
};

#endif
