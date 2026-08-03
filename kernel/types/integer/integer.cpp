#include "integer.h"
#include "string.h"


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
        result = (result * base) + (text[i] - '0');
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
