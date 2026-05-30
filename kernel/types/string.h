#ifndef STRING_H
#define STRING_H

#include <stdint.h>

char* ppm_malloc_str(const uint64_t size);
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

#endif
