#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>
#include "color.h"

extern int FONT_SIZE;

extern int cursorAt_X;
extern int cursorAt_Y;

// PRINT
void print(const char* text, Color color);
void print(const char* text);

void print_inline(const char* text, Color color);
void print_inline(const char* text);

void print_chars(const char* text, Color color);
void print_char(const char c);

void newline();

void init_print();

#endif
