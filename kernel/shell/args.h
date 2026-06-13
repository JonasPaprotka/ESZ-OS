#ifndef ARGS_H
#define ARGS_H

#include "config.h"
#include <stdint.h>

const char* getLastTokenFromBuffer(const uint64_t inputLength, const char inputBuffer[TERMINAL_BUFFER_SIZE]);

const char* getInputArgs(const uint64_t inputLength, char inputBuffer[TERMINAL_BUFFER_SIZE]);
bool executeCommand(uint64_t inputLength, char inputBuffer[TERMINAL_BUFFER_SIZE], const char* args);

#endif
