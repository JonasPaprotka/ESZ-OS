#ifndef TERMINAL_H
#define TERMINAL_H

#include "config.h"
#include <stdint.h>

extern uint64_t cmdHistCount;
extern char commandHistory[MAX_COMMAND_HISTORY][TERMINAL_BUFFER_SIZE];

extern unsigned int lineInputCursorPos;
extern unsigned int lineInputStart_X;

void cursor_move_inline(const bool move_right);
void handle_input_buffer_deletion();
void insert_char_at_cursor(const char c);

void terminal_init();
void terminal_on_key(unsigned char scancode);

#endif
