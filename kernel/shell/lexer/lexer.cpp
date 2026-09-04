#include "lexer.h"
#include <stdint.h>
#include "commands.h" //TODO remove cross ref - maybe add smth like COMMAND REGISTRY
#include "string.h"
#include "integer.h"
#include "boolean.h"

static LexerWord lex_word_classifier(const char* text) {
    switch (text[0]) {
        case '#':
            return LexerWord::COMMENT;
        case '"':
            return LexerWord::TEXT_VALUE;
    }

    if (is_integer(text[0])) return LexerWord::INTEGER_VALUE;
    if (text[0] == '-') return LexerWord::ARGUMENT_FLAG;

    if (is_bool(text)) return LexerWord::BOOL_VALUE;

    return LexerWord::UNKNOWN;
}

static void process_lexed_text(char* lexed_text[], const uint64_t lexed_qty, LexerWord OUT_lexed_words[]) {
    if (lexed_qty == 0) return;

    for (uint64_t i = 0; i < lexed_qty; i++) {
        OUT_lexed_words[i] = lex_word_classifier(lexed_text[i]);
    }
}


void lex_text(const char* text, char* OUT_lexed_text[], LexerWord OUT_lexed_words[], uint64_t &OUT_lexed_qty) {
    //const uint64_t text_len = str_length(text);

    str_split(text, ' ', OUT_lexed_text, OUT_lexed_qty); //TODO this is temporary

    process_lexed_text(OUT_lexed_text, OUT_lexed_qty, OUT_lexed_words);
}
