#pragma once

#include "color.h"

// --- GLOBALS ---
extern int FONT_W;
extern int FONT_H;
extern int ADJUSTED_WIDTH;
extern int MAX_CHARS;
extern int MAX_LINES;

extern int cursorAt_X;
extern int cursorAt_Y;
extern int cursorRendered_X;
extern int cursorRendered_Y;
extern bool cursor_visible;

extern bool useScreenBuffer;
extern bool isRedrawing;


// --- CURSOR ---
void update_cursor_render();


// --- LINE MGT ---
void newline();
void prevline();


// --- PRINT CHAR ---
void print_char(const char c, const Color color, const bool interactable);
void print_char(const char c, const bool interactable);
void print_char(const char c);


// --- PRINT CHARS ---
void print_chars(const char* text, const Color color, const bool interactable);
void print_chars(const char* text, const Color color);
void print_chars(const char* text, const bool interactable);


// --- PRINT INLINE ---
void print_inline(const char* text, const Color color);
void print_inline(const char* text);


// --- PRINT ---
void print(const char* text, const Color color);
void print(const char* text);


// --- REDRAW ---
void redraw_line(const int line);

// --- INIT ---
void init_print();
