#ifndef TERMINAL_H
#define TERMINAL_H

#include "config.h"
#include <stdint.h>

extern uint64_t cmdHistCount;
extern char commandHistory[MAX_COMMAND_HISTORY][TERMINAL_BUFFER_SIZE];

void terminal_init();
void terminal_on_key(unsigned char scancode);

#endif
