#pragma once

#include "config.h"
#include <stdint.h>

extern uint64_t lineInputLength;
extern char lineInputBuffer[TERMINAL_BUFFER_SIZE];
extern unsigned int lineInputCursorPos;
extern unsigned int lineInputStart_X;

void cursor_move_inline(const bool move_right);
void handle_input_buffer_deletion();
void insert_char_at_cursor(const char c);
void clear_input_on_screen();
void replaceCurrentToken(const char* oldToken, const char* newToken);
void newTerminalInputLine();

void terminal_init();
void terminal_on_key(uint8_t scancode);
