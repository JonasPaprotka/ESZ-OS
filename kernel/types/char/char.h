#pragma once

inline bool is_digit(const char c) {
    return (c >= '0' && c <= '9');
}

inline bool is_alpha(const char c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

inline bool is_upper_alpha(const char c) {
    return (c >= 'A' && c <= 'Z');
}

inline bool is_lower_alpha(const char c) {
    return (c >= 'a' && c <= 'z');
}

inline bool is_whitespace(const char c) {
    // 9 = tab
    return (c == 32 || c == 9);
}

inline char to_upper(const char c) {
    if (!is_lower_alpha(c)) return c;
    return c - 32;
}

inline char to_lower(const char c) {
    if (!is_upper_alpha(c)) return c;
    return c + 32;
}
