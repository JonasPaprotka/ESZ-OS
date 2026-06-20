#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>
#include "color.h"

extern int FONT_W;
extern int FONT_H;
extern int ADJUSTED_WIDTH;

extern int cursorAt_X;
extern int cursorAt_Y;

extern bool cursor_visible;

extern int cursorRendered_X;
extern int cursorRendered_Y;

extern int MAX_CHARS;
extern int MAX_LINES;

extern bool useScreenBuffer;
extern bool isRedrawing;

void handle_scroll();
void update_cursor_render();

// PRINT
void draw_char(const char c, const int printAt_X, const int printAt_Y, const Color color, const bool interactable);

void print(const char* text, const Color color);
void print(const char* text);

void print_inline(const char* text, const Color color);
void print_inline(const char* text);

void print_chars(const char* text, const Color color);
void print_chars(const char* text, const bool interactable);

void print_char(const char c);
void print_char(const char c, const bool interactable);

void newline();

void delete_unprotected_chars();
void cursor_backspace();

void redraw_line(const int line);

void init_print();

#endif // PRINT_H
