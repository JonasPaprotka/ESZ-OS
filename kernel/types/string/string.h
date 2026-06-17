#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include "memory.h"

char* malloc_str(const uint64_t size);
char* malloc_empty_str();
uint64_t str_length(const char* str);
char* str_repeat(const char* text, uint64_t amount);
bool str_equal(const char* a, const char* b);
void str_copy(char* dest, const char* src);
void str_add(char* target, const char* value);
void str_add(char* target, const char char_value);
char* str_combine(const char* a, const char* b);
bool str_contains(const char* text, const char* searchText);
uint64_t str_count(const char* text, const char* searchText);
bool str_starts_with(const char* text, const char* searchText);
bool str_ends_with(const char* text, const char* searchText);
void str_replace(char* text, const char* searchText, const char* replacedBy);
char* to_string(const uint64_t inputValue, const uint8_t basis);
char* to_string(const int64_t inputValue, const uint8_t basis);
char* to_string(const uint64_t inputValue);
char* to_string(const int64_t inputValue);
char* to_string(const int32_t inputValue);
char* to_string(const uint32_t inputValue);
void str_cut_end(char* Text, const uint64_t n);
char* str_cut_start(const char* text, const uint64_t n);
char* str_move_right(const char* text, const uint64_t moveAmount);

char* str_trim_start(const char* text);
char* str_trim_end(const char* text);
char* str_trim(const char* text);
char* str_to_upper(const char* text);
char* str_to_lower(const char* text);

struct String {
    char* data;
    
    static const char* to_cstr(const char* s) {
        char* ret = malloc_str(str_length(s) + 1);
        str_copy(ret, s);
        return ret;
    }
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
            free((char*) parts[i]);
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
