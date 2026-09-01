#include "history.h"
#include "config.h"
#include "integer.h"
#include "string.h"
#include "print.h"
#include "line_editor.h"

char commandHistory[MAX_COMMAND_HISTORY][LINE_EDITOR_BUFFER_SIZE];
uint64_t cmdHistCount = 0;
uint64_t goThroughHistoryCount = 0;

void add_command_to_history(char command[LINE_EDITOR_BUFFER_SIZE]) {
    if (cmdHistCount >= MAX_COMMAND_HISTORY) {
        for (uint64_t i = 0; i < cmdHistCount - 1; i++) {
            str_copy(commandHistory[i], commandHistory[i + 1]);
        }
        commandHistory[cmdHistCount - 1][0] = 0;
        cmdHistCount--;
    }

    str_copy(commandHistory[cmdHistCount], command);
    cmdHistCount++;
}

void handle_show_history() {
    clear_input_on_screen();
    str_copy(lineInputBuffer, commandHistory[cmdHistCount - goThroughHistoryCount]);
    lineInputLength = str_length(lineInputBuffer);
    lineInputCursorPos = lineInputLength;
    print_chars(lineInputBuffer, true);
}
