#ifndef STRING_H
#define STRING_H

typedef const char* cstr;
typedef char* str;

int str_length(cstr str);

bool str_equal(cstr a, cstr b);

void str_copy(cstr src, str dest);
void str_add(str target, cstr value);
str str_combine(cstr a, cstr b);
str str_repeat(cstr text, int amount);

bool str_contains(cstr text, cstr searchText);
int str_count(cstr text, cstr searchText);
bool str_starts_with(cstr text, cstr searchText);
bool str_ends_with(cstr text, cstr searchText);
void str_replace(str text, cstr toBeReplacedText, cstr replacementText);

str to_string(const int inputValue);

#endif
