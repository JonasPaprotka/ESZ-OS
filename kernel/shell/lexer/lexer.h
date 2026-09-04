#pragma once

#include <stdint.h>

enum class LexerWord {
    COMMAND,
    COMMAND_OPERATOR,

    ARGUMENT_FLAG,

    INTEGER_VALUE,
    TEXT_VALUE,
    BOOL_VALUE,

    COMMENT,
    UNKNOWN
};

enum class LexerToken {
    GREATER_THAN,
    GREATER_EQUAL,
    LESS_THAN,
    LESS_EQUAL,
    EQUAL,
    NOT_EQUAL,

    NOT,
    OR,
    AND,
    XOR,
    XNOR,

    PARENTHESIS_OPEN,
    PARENTHESIS_CLOSE,

    MINUS,
    PLUS,
    DIVISION,
    MULTIPLICATION
};

void lex_text(const char* text, char* OUT_lexed_text[], LexerWord OUT_lexed_words[], uint64_t &OUT_lexed_qty);

