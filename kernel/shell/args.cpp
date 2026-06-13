#include "config.h"
#include "print.h"
#include "info_text.h"
#include "commands.h"

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

bool executeCommand(uint64_t inputLength, char inputBuffer[TERMINAL_BUFFER_SIZE], const char* args) {
    for (uint64_t i = 0; commands[i].name != 0; i++) {
        if (str_equal(inputBuffer, commands[i].name)) {
            newline();
            inputLength = 0;
            inputBuffer[0] = 0;
            commands[i].execute(args);
            return true;
        }
    }

    return false;
}
