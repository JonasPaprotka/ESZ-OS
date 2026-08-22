#include "integer.h"
#include "char.h"
#include "string.h"


uint64_t digit_value(const char c) {
    if (is_digit(c)) return c - '0';
    if (is_alpha(c)) return to_lower(c) - 'a' + 10;
    return 0;
}

int64_t to_int(const char* text, const uint64_t base) {
    const uint64_t text_len = str_length(text);
    if (text[0] == 0) return 0;

    bool isNegative = false;
    uint64_t numberStartAtIdx = 0;

    if (text[0] == '-') {
        isNegative = true;
        numberStartAtIdx = 1;
    }

    int64_t result = 0;
    for (uint64_t i = numberStartAtIdx; i < text_len; i++) {
        result = (result * base) + digit_value(text[i]);
    }

    if (isNegative) {
        return 0 - result;
    } else {
        return result;
    }
}

int64_t to_int(const char* text) {
    return to_int(text, 10);
}
