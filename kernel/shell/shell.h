#pragma once

#include <stdint.h>

void new_line_editor_input_line();
void display_line_editor_error(const char* Text);

void shell_on_key(uint8_t scancode);
bool shell_init();
