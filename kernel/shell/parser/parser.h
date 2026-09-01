#pragma once

#include "config.h"
#include <stdint.h>
#include "return.h"

const char* getCurrTokenFromBuffer(const int inputBufferCursorAt_X, const uint64_t inputBufferLength, const char inputBuffer[LINE_EDITOR_BUFFER_SIZE]);

const char* getInputArgs(const uint64_t inputLength, char inputBuffer[LINE_EDITOR_BUFFER_SIZE]);
Return executeCommand(char inputBuffer[LINE_EDITOR_BUFFER_SIZE], const char* args);
