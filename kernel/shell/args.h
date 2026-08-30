#pragma once

#include "config.h"
#include <stdint.h>
#include "types.h"
#include "return.h"

struct Argument {
    const char* name;
    const char* summary;
    Types type;
    bool isRequired;
    char value[TERMINAL_BUFFER_SIZE];
};

const char* getCurrTokenFromBuffer(const int inputBufferCursorAt_X, const uint64_t inputBufferLength, const char inputBuffer[TERMINAL_BUFFER_SIZE]);

const char* getInputArgs(const uint64_t inputLength, char inputBuffer[TERMINAL_BUFFER_SIZE]);
Return executeCommand(char inputBuffer[TERMINAL_BUFFER_SIZE], const char* args);
