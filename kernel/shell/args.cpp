#include "config.h"
#include "print.h"
#include "info_text.h"
#include "commands.h"

const char* getLastTokenFromBuffer(const uint64_t inputLength, const char inputBuffer[TERMINAL_BUFFER_SIZE]) {
    uint64_t lastSpaceAt = 0;

    for (uint64_t i = 0; i < inputLength; i++) {
        if (inputBuffer[i] == ' ') lastSpaceAt = i;
    }

    if (lastSpaceAt != 0) {
        return &inputBuffer[lastSpaceAt + 1];
    }

    return &inputBuffer[0];
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
