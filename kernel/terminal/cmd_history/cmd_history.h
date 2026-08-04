#ifndef CMD_HISTORY_H
#define CMD_HISTORY_H

#include "config.h"
#include <stdint.h>

extern uint64_t goThroughHistoryCount;
extern uint64_t cmdHistCount;
extern char commandHistory[MAX_COMMAND_HISTORY][TERMINAL_BUFFER_SIZE];

void add_command_to_history(char command[TERMINAL_BUFFER_SIZE]);
void handle_show_history();

#endif // CMD_HISTORY_H
