#include "config.h"
#include "print.h"
#include "info_text.h"
#include "commands.h"

const char* getCurrTokenFromBuffer(const int inputBufferCursorAt_X, const uint64_t inputBufferLength, const char inputBuffer[TERMINAL_BUFFER_SIZE]) {
    uint64_t tokenStartsAt = 0;

    for (uint64_t i = inputBufferCursorAt_X; i > 0; i--) {
        if (inputBuffer[i] == ' ' and i != inputBufferCursorAt_X) {
            tokenStartsAt = i;
            break;
        }
    }

    // when space found -> starts at next char
    if (tokenStartsAt != 0) tokenStartsAt++;

    char* retToken = malloc_str(inputBufferLength + 1);
    for (uint64_t i = tokenStartsAt; i < inputBufferLength; i++) {
        if (inputBuffer[i] == ' ') return retToken;
        str_add(retToken, inputBuffer[i]);
    }

    return retToken;
}

const char* getInputArgs(const uint64_t inputLength, char inputBuffer[TERMINAL_BUFFER_SIZE]) {
    const char* args = "";
    for (uint64_t i = 0; i < inputLength; i++) {
        if (inputBuffer[i] == ' ') {
            inputBuffer[i] = 0;
            args = &inputBuffer[i+1];
            break;
        }
    }

    return args;
}

bool executeCommand(char inputBuffer[TERMINAL_BUFFER_SIZE], const char* args) {
    for (uint64_t i = 0; commands[i].name != 0; i++) {
        if (str_equal(inputBuffer, commands[i].name)) {
            newline();
            commands[i].execute(args);
            return true;
        }
    }

    return false;
}
