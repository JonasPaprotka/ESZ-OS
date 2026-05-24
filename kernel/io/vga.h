#ifndef VGA_H
#define VGA_H

#include "string.h"

enum Color {
    Black   = 0x00,
    Blue    = 0x01,
    Green   = 0x02,
    Cyan    = 0x03,
    Red     = 0x04,
    Magenta = 0x05,
    Brown   = 0x06,
    White   = 0x07,
    DarkGray    = 0x08,
    LightBlue   = 0x09,
    LightGreen  = 0x0A,
    LightCyan   = 0x0B,
    LightRed    = 0x0C,
    LightMagenta= 0x0D,
    Yellow      = 0x0E,
    BrightWhite = 0x0F
};

void print_char(char c, enum Color color, int pos);
void print_char(char c);

void print(const string text, enum Color textColor, int line, int padding);
void print(const string text, enum Color textColor);
void print(const string text);

void print_inline(const string text, enum Color textColor);
void print_inline(const string text);

void clear();
void set_cursor(int pos);
void clear_char(int pos);

#endif
