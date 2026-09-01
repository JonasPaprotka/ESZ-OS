#pragma once

#include "config.h"
#include <stdint.h>

extern uint64_t goThroughHistoryCount;
extern uint64_t cmdHistCount;
extern char commandHistory[MAX_COMMAND_HISTORY][LINE_EDITOR_BUFFER_SIZE];

void add_command_to_history(char command[LINE_EDITOR_BUFFER_SIZE]);
void handle_show_history();
