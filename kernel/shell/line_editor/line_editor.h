#pragma once

#include "config.h"
#include <stdint.h>

extern uint64_t lineInputLength;
extern char lineInputBuffer[LINE_EDITOR_BUFFER_SIZE];
extern unsigned int lineInputCursorPos;
extern unsigned int lineInputStart_X;

void cursor_move_inline(const bool move_right);
void handle_input_buffer_deletion();
void clear_input_on_screen();
void replace_curr_token(const char* oldToken, const char* newToken);
void reset_line_input();
void insert_char_at_cursor(const char c);
