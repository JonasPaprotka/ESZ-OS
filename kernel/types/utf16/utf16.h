#ifndef UTF16_H
#define UTF16_H

#include <stdint.h>

char utf16_char_to_ascii(const char c[2]);
char* utf16_text_to_ascii(const char* text, const uint32_t charCount);

#endif // UTF16_H
