#pragma once

#include "config.h"
#include <stdint.h>

extern uint64_t lineInputLength;
extern char lineInputBuffer[TERMINAL_BUFFER_SIZE];
extern unsigned int lineInputCursorPos;
extern unsigned int lineInputStart_X;

void cursor_move_inline(const bool move_right);
void handle_input_buffer_deletion();
void clear_input_on_screen();
void replace_curr_token(const char* oldToken, const char* newToken);
void new_line_editor_input_line();

void reset_line_input();
void display_line_editor_error(const char* Text);
void insert_char_at_cursor(const char c);

bool line_editor_init();
