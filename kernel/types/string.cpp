#include "string.h"
#include "memory.h"

str malloc_str(const unsigned int size) {
    unsigned int addr = malloc(128);
    str string = (str) addr;
    memory_clear(string, 128);
    return string;
}

int str_length(cstr str) {
    int i = 0;
    while (str[i] != 0) i++;
    return i;
}

bool str_equal(cstr a, cstr b) {
    const int a_len = str_length(a);
    const int b_len = str_length(b);
    
    if (a_len != b_len) return false;

    for (int i = 0; i < a_len; ++i) {
        if (a[i] != b[i]) return false;
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
    const int searchText_len = str_length(searchText);
    const int text_len = str_length(text);
    if (searchText_len > text_len) return 0;

    for (int i = 0; i < text_len - searchText_len; ++i) {
        str comparison = "";
        for (int j = 0; j < searchText_len; ++j) {
            comparison[j] = text[i + j];
        }
        comparison[searchText_len] = 0;
        if (searchText == comparison) return true;
    }
    return false;
}

int str_count(cstr text, cstr searchText) {
    int counterHits = 0;
    const int searchText_len = str_length(searchText);
    const int text_len = str_length(text);
    if (searchText_len > text_len) return 0;

    for (int i = 0; i < text_len - searchText_len; ++i) {
        str comparison = "";
        for (int j = 0; j < searchText_len; ++j) {
            comparison[j] = text[i + j];
        }
        comparison[searchText_len] = 0;
        if (searchText == comparison) counterHits++;
    }
    return counterHits;
}

bool str_starts_with(cstr text, cstr searchText) {
    const int searchText_len = str_length(searchText);
    if (searchText_len > str_length(text)) return false;

    for (int i = 0; i < searchText_len; ++i) {
        if (text[i] != searchText[i]) return false;
    }
    return true;
}

bool str_ends_with(cstr text, cstr searchText) {
    const int searchText_len = str_length(searchText);
    if (searchText_len > str_length(text)) return false;

    for (int i = searchText_len; i > 0; --i) {
        if (text[i] != searchText[i]) return false;
    }
    return true;
}

void str_replace(str text, cstr toBeReplacedText, cstr replacementText) {
    //TODO
}

str str_repeat(cstr text, int amount) {
    str returnString = malloc_str(128);
    const int text_len = str_length(text);

    for (int i = 0; i < amount; i++) {
        for (int j = 0; j < text_len; j++) {
            returnString[i * text_len + j] = text[j];
        }
    }

    returnString[text_len * amount] = 0;
    return returnString;
}

str str_combine(cstr a, cstr b) {
    str returnString = malloc_str(128);

    const int len_a = str_length(a);
    const int len_b = str_length(b);

    for (int i = 0; i < len_a; i++) {
        returnString[i] = a[i];
    }
    for (int i = 0; i < len_b; i++) {
        returnString[i + len_a] = b[i];
    }
    
    returnString[len_a + len_b] = 0;
    return returnString;
}

str to_string(const int inputValue) {
    str composedString = malloc_str(12);
    str returnString = malloc_str(12);

    int strLength = 0;
    int calcValue = inputValue;

    while (calcValue != 0) {
        composedString[strLength] = calcValue % 10 + '0'; // + '0' is to make ascii
        calcValue /= 10;
        strLength++;
    }

    int counter = 0;
    for (int i = strLength - 1; i >= 0; --i) {
        returnString[counter] = composedString[i];
        ++counter;
    }

    returnString[strLength] = 0;
    return returnString;
}
