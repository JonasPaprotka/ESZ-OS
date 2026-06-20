#ifndef ARGS_H
#define ARGS_H

#include "config.h"
#include <stdint.h>

const char* getCurrTokenFromBuffer(const int inputBufferCursorAt_X, const uint64_t inputBufferLength, const char inputBuffer[TERMINAL_BUFFER_SIZE]);

const char* getInputArgs(const uint64_t inputLength, char inputBuffer[TERMINAL_BUFFER_SIZE]);
bool executeCommand(char inputBuffer[TERMINAL_BUFFER_SIZE], const char* args);

#endif // ARGS_H
