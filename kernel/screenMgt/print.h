#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>
#include "color.h"

extern int FONT_SIZE;
extern int FONT_W;
extern int FONT_H;

extern int cursorAt_X;
extern int cursorAt_Y;

extern int MAX_CHARS;
extern int MAX_LINES;

extern bool useScreenBuffer;
extern bool isRedrawing;

void handle_scroll();
uint64_t calc_line_pixels();

// PRINT
void print(const char* text, Color color);
void print(const char* text);

void print_inline(const char* text, Color color);
void print_inline(const char* text);

void print_chars(const char* text, Color color);
void print_chars(const char* text, bool interactable);
void print_char(const char c);

void newline();

void delete_unprotected_chars();
void cursor_backspace();

void init_print();

#endif
