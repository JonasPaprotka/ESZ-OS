#include "string.h"

int str_length(cstr str) {
    int i = 0;
    while (str[i] != 0) i++;
    return i;
}

bool str_equal(cstr a, cstr b) {
    const int a_len = str_length(a);
    const int b_len = str_length(b);
    
    if (a_len != b_len) { return false; }

    for (int i = 0; i < a_len; ++i) {
        if (a[i] != b[i]) { return false; }
    }
    return true;
}

void str_copy(cstr src, str dest) {
    const int src_len = str_length(src);

    for (int i = 0; i < src_len; ++i) {
        dest[i] = src[i];
    }
    dest[src_len] = 0;
}

void str_add(str target, cstr value) {
    const int target_length = str_length(target);
    const int value_length = str_length(value);

    for (int i = 0; i < value_length; ++i) {
        target[target_length + i] = value[i];
    }
    target[target_length + value_length] = 0;
}

bool str_contains(cstr text, cstr searchText) {
    //TODO
    return false;
}

int str_count(cstr text, cstr searchText) {
    //TODO
    return 0;
}

bool str_starts_with(cstr text, cstr searchText) {
    //TODO
    return false;
}

bool str_ends_with(cstr text, cstr searchText) {
    //TODO
    return false;
}

void str_replace(str text, cstr toBeReplacedText, cstr replacementText) {
    //TODO
}

str to_string(int inputValue) {
    //TODO
    return "";
}
