#include "boolean.h"
#include <stdint.h>
#include "string.h"
#include "char.h"
#include "heap.h"

bool as_bool(const uint64_t value) {
    return (value == 1);
}

bool as_bool(const char c) {
    const char lower_c = to_lower(c);
    return (lower_c == 't' || lower_c == 'y' || lower_c == '1');
}

bool as_bool(const char* text) {
    const char* lower_text = str_to_lower(text);
    const bool bool_value = (str_equal(lower_text, "true") || str_equal(lower_text, "yes") || str_equal(lower_text, "1"));
    free(lower_text);
    return bool_value;
}

bool is_bool(const uint64_t value) {
    return (value == 1 || value == 0);
}

bool is_bool(const char c) {
    const char lower_c = to_lower(c);
    return (lower_c == 't' || lower_c == 'y' || lower_c == '1' || lower_c == 'f' || lower_c == 'n' || lower_c == '0');
}

bool is_bool(const char* text) {
    const char* lower_text = str_to_lower(text);
    const bool is_bool = (str_equal(lower_text, "true") || str_equal(lower_text, "false") ||
                            str_equal(lower_text, "yes") || str_equal(lower_text, "no") ||
                            str_equal(lower_text, "1") || str_equal(lower_text, "0"));
    free(lower_text);
    return is_bool;
}
