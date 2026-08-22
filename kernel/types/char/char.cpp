#include "char.h"

bool is_digit(const char c) {
    return (c >= '0' && c <= '9');
}

bool is_alpha(const char c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

bool is_upper_alpha(const char c) {
    return (c >= 'A' && c <= 'Z');
}

bool is_lower_alpha(const char c) {
    return (c >= 'a' && c <= 'z');
}

bool is_whitespace(const char c) {
    // 9 = tab
    return (c == 32 || c == 9);
}

char to_upper(const char c) {
    if (!is_lower_alpha(c)) return c;
    return c - 32;
}

char to_lower(const char c) {
    if (!is_upper_alpha(c)) return c;
    return c + 32;
}
