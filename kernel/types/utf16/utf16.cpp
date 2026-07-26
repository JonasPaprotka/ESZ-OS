#include "string.h"
#include <stdint.h>

char utf16_char_to_ascii(const char c[2]) {
    return c[0];
}

char* utf16_text_to_ascii(const char* text, const uint32_t charCount) {
    char* returnString = malloc_str(charCount + 1);

    uint32_t writtenChars = 0;
    for (uint32_t i = 0; i < charCount; i++) {
        const uint8_t low = (uint8_t)text[i * 2];
        const uint8_t high = (uint8_t)text[i * 2 + 1];

        if ((low == 0x00 && high == 0x00) || (low == 0xFF && high == 0xFF)) break;

        returnString[writtenChars] = low;
        writtenChars++;
    }

    returnString[writtenChars] = 0;
    return returnString;
}
