#include "heap.h"
#include "integer.h"
#include "commands.h"
#include "string.h"
#include "print.h"
#include "line_editor.h"
#include "args.h"

void handleTabAutoCompletion() {
    if (lineInputLength == 0) return;

    uint64_t maxCommandCounter = 0;
    for (uint64_t i = 0; commands[i].name != 0; i++) {
        maxCommandCounter++;
    }

    const char* currentToken = getCurrTokenFromBuffer(lineInputCursorPos, lineInputLength, lineInputBuffer);
    if (currentToken[0] == 0) {
        free(currentToken);
        return;
    }

    uint64_t validTabCannidates = 0;
    Command cannidateList[maxCommandCounter];

    // match input to list
    for (uint64_t i = 0; commands[i].name != 0; i++) {
        if (str_starts_with(commands[i].name, currentToken)) {
            cannidateList[validTabCannidates] = commands[i];
            validTabCannidates++;
        }
    }

    if (validTabCannidates == 0) {
        free(currentToken);
        return;
    }

    // == 1 -> autocomplete directly
    if (validTabCannidates == 1) {
        replaceCurrentToken(currentToken, cannidateList[0].name);
        free(currentToken);
        return;
    };

    free(currentToken);

    // > 1 -> show possibilities in newline and refill the input in another newline
    // more tabs -> rotate through
    if (validTabCannidates > 1) {
        newline();
        for(uint64_t i = 0; i < validTabCannidates; i++) {
            print_chars(cannidateList[i].name, false);
            if (i != validTabCannidates - 1) print_chars(" | ", false);
        }

        newline();
        newTerminalInputLine();
        print_chars(lineInputBuffer, true);
        lineInputCursorPos = lineInputLength;

        //TODO rotation

        return;
    }
}
