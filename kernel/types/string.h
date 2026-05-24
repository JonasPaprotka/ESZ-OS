#ifndef STRING_H
#define STRING_H

typedef const char* cstr;
typedef char* str;

int str_length(cstr str);
bool str_equal(cstr a, cstr b);
void str_copy(cstr src, str dest);
void str_add(str target, cstr value);

#endif
