#include "string.h"
#include "memory.h"
#include <stdint.h>

char* malloc_str(const uint64_t size) {
    char* string = (char*) malloc(size);
    memory_clear(string, size);
    return string;
}

char* malloc_empty_str() {
    char* ret = malloc_str(2);
    ret[0] = '0';
    ret[1] = 0;
    return ret;
}

uint64_t str_length(const char* str) {
    uint64_t i = 0;
    while (str[i] != 0) i++;
    return i;
}

bool str_equal(const char* a, const char* b) {
    const uint64_t a_len = str_length(a);
    const uint64_t b_len = str_length(b);
    
    if (a_len != b_len) return false;

    for (uint64_t i = 0; i < a_len; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

void str_copy(char* dest, const char* src) {
    const uint64_t src_len = str_length(src);

    for (uint64_t i = 0; i < src_len; ++i) {
        dest[i] = src[i];
    }
    dest[src_len] = 0;
}

void str_add(char* target, const char* value) {
    const uint64_t target_length = str_length(target);
    const uint64_t value_length = str_length(value);

    for (uint64_t i = 0; i < value_length; ++i) {
        target[target_length + i] = value[i];
    }
    target[target_length + value_length] = 0;
}

void str_add(char* target, const char char_value) {
    const uint64_t target_length = str_length(target);

    for (uint64_t i = 0; i < 1; ++i) {
        target[target_length + i] = char_value;
    }
    target[target_length + 1] = 0;
}

bool str_contains(const char* text, const char* searchText) {
    const uint64_t searchText_len = str_length(searchText);
    const uint64_t text_len = str_length(text);

    if (searchText_len > text_len) return 0;

    char comparison[searchText_len + 1];

    for (uint64_t i = 0; i <= text_len - searchText_len; i++) {
        for (uint64_t j = 0; j < searchText_len; j++) {
            comparison[j] = text[i + j];
        }
        comparison[searchText_len] = 0;
        if (str_equal(searchText, comparison)) return true;
    }
    return false;
}

uint64_t str_count(const char* text, const char* searchText) {
    uint64_t counterHits = 0;
    const uint64_t searchText_len = str_length(searchText);
    const uint64_t text_len = str_length(text);

    if (searchText_len > text_len) return 0;

    char comparison[searchText_len + 1];

    for (uint64_t i = 0; i <= text_len - searchText_len; i++) {
        for (uint64_t j = 0; j < searchText_len; j++) {
            comparison[j] = text[i + j];
        }
        comparison[searchText_len] = 0;
        if (str_equal(searchText, comparison)) counterHits++;
    }
    return counterHits;
}

bool str_starts_with(const char* text, const char* searchText) {
    const uint64_t searchText_len = str_length(searchText);
    if (searchText_len > str_length(text)) return false;

    for (uint64_t i = 0; i < searchText_len; ++i) {
        if (text[i] != searchText[i]) return false;
    }
    return true;
}

bool str_ends_with(const char* text, const char* searchText) {
    const uint64_t searchText_len = str_length(searchText);
    if (searchText_len > str_length(text)) return false;

    for (uint64_t i = searchText_len; i > 0; --i) {
        if (text[i] != searchText[i]) return false;
    }
    return true;
}

/* void str_replace(char* text, const char* toBeReplacedText, const char* replacementText) {
    //TODO
} */

char* str_repeat(const char* text, uint64_t amount) {
    const uint64_t text_len = str_length(text);
    char* returnString = malloc_str(text_len * amount + 1);

    for (uint64_t i = 0; i < amount; i++) {
        for (uint64_t j = 0; j < text_len; j++) {
            returnString[i * text_len + j] = text[j];
        }
    }

    returnString[text_len * amount] = 0;
    return returnString;
}

char* str_combine(const char* a, const char* b) {
    const uint64_t len_a = str_length(a);
    const uint64_t len_b = str_length(b);

    char* returnString = malloc_str(len_a + len_b + 1);

    for (uint64_t i = 0; i < len_a; i++) {
        returnString[i] = a[i];
    }
    for (uint64_t i = 0; i < len_b; i++) {
        returnString[i + len_a] = b[i];
    }
    
    returnString[len_a + len_b] = 0;
    return returnString;
}


char* to_string(const uint64_t inputValue, const uint8_t basis) {
    if (inputValue == 0 || basis == 0 || basis == 1) {
        return malloc_empty_str();
    }

    uint64_t strLength = 0;
    uint64_t calcValue = inputValue;

    while (calcValue != 0) {
        calcValue /= basis;
        strLength++;
    }

    char* composedString = malloc_str(strLength + 1);
    char* returnString = malloc_str(strLength + 1);

    calcValue = inputValue;
    strLength = 0;
    while (calcValue != 0) {
        uint64_t newValue = calcValue % basis;
        if (newValue < 10) {
            composedString[strLength] = newValue + '0'; // make ascii
        } else {
            composedString[strLength] = 'a' + (newValue - 10); // make ascii
        }
        calcValue /= basis;
        strLength++;
    }

    uint64_t counter = 0;
    for (int64_t i = strLength - 1; i >= 0; --i) {
        returnString[counter] = composedString[i];
        ++counter;
    }

    free(composedString);
    returnString[strLength] = 0;
    return returnString;
}

char* to_string(const int64_t inputValue, const uint8_t basis) {
    if (inputValue >= 0) return to_string((uint64_t)inputValue, basis);
    return str_combine("-", to_string((uint64_t)(-(inputValue)), basis));
}

char* to_string(const uint64_t inputValue) {
    return to_string(inputValue, 10);
}

char* to_string(const int64_t inputValue) {
    return to_string(inputValue, 10);
}

char* to_string(const int32_t inputValue) {
    return to_string((int64_t)inputValue, 10);
}

char* to_string(const uint32_t inputValue) {
    return to_string((uint64_t)inputValue, 10);
}


void str_trim_end(char* Text, const uint64_t n) {
    uint64_t text_length = str_length(Text);
    if (n >= text_length) {
        Text[0] = 0;
        return;
    }

    Text[text_length - n] = 0;
}
