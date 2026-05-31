#include "string.h"
#include "memory.h"
#include <stdint.h>

char* malloc_str(const uint64_t size) {
    char* string = (char*) malloc(size);
    memory_clear(string, size);
    return string;
}

int str_length(const char* str) {
    int i = 0;
    while (str[i] != 0) i++;
    return i;
}

bool str_equal(const char* a, const char* b) {
    const int a_len = str_length(a);
    const int b_len = str_length(b);
    
    if (a_len != b_len) return false;

    for (int i = 0; i < a_len; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

void str_copy(char* dest, const char* src) {
    const int src_len = str_length(src);

    for (int i = 0; i < src_len; ++i) {
        dest[i] = src[i];
    }
    dest[src_len] = 0;
}

void str_add(char* target, const char* value) {
    const int target_length = str_length(target);
    const int value_length = str_length(value);

    for (int i = 0; i < value_length; ++i) {
        target[target_length + i] = value[i];
    }
    target[target_length + value_length] = 0;
}

bool str_contains(const char* text, const char* searchText) {
    //TODO fix/rewrite
    const int searchText_len = str_length(searchText);
    const int text_len = str_length(text);
    if (searchText_len > text_len) return 0;

    for (int i = 0; i < text_len - searchText_len; ++i) {
        char* comparison = "";
        for (int j = 0; j < searchText_len; ++j) {
            comparison[j] = text[i + j];
        }
        comparison[searchText_len] = 0;
        if (searchText == comparison) return true;
    }
    return false;
}

int str_count(const char* text, const char* searchText) {
    //TODO fix/rewrite
    int counterHits = 0;
    const int searchText_len = str_length(searchText);
    const int text_len = str_length(text);
    if (searchText_len > text_len) return 0;

    for (int i = 0; i < text_len - searchText_len; ++i) {
        char* comparison = "";
        for (int j = 0; j < searchText_len; ++j) {
            comparison[j] = text[i + j];
        }
        comparison[searchText_len] = 0;
        if (searchText == comparison) counterHits++;
    }
    return counterHits;
}

bool str_starts_with(const char* text, const char* searchText) {
    const int searchText_len = str_length(searchText);
    if (searchText_len > str_length(text)) return false;

    for (int i = 0; i < searchText_len; ++i) {
        if (text[i] != searchText[i]) return false;
    }
    return true;
}

bool str_ends_with(const char* text, const char* searchText) {
    const int searchText_len = str_length(searchText);
    if (searchText_len > str_length(text)) return false;

    for (int i = searchText_len; i > 0; --i) {
        if (text[i] != searchText[i]) return false;
    }
    return true;
}

void str_replace(char* text, const char* toBeReplacedText, const char* replacementText) {
    //TODO
}

char* str_repeat(const char* text, int amount) {
    char* returnString = malloc_str(128);
    const int text_len = str_length(text);

    for (int i = 0; i < amount; i++) {
        for (int j = 0; j < text_len; j++) {
            returnString[i * text_len + j] = text[j];
        }
    }

    returnString[text_len * amount] = 0;
    return returnString;
}

char* str_combine(const char* a, const char* b) {
    char* returnString = malloc_str(128);

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


char* to_string(const uint64_t inputValue, const uint8_t basis) {
    if (inputValue == 0 || basis == 0 || basis == 1) {
        char* emptyReturnString = malloc_str(2);
        emptyReturnString[0] = '0';
        emptyReturnString[1] = 0;
        return emptyReturnString;
    }

    char* composedString = malloc_str(65);
    char* returnString = malloc_str(65);
    uint64_t strLength = 0;
    uint64_t calcValue = inputValue;

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

    int counter = 0;
    for (int i = strLength - 1; i >= 0; --i) {
        returnString[counter] = composedString[i];
        ++counter;
    }

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
