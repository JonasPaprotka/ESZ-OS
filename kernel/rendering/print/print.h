#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>
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
void render_cursor(const int printAt_X, const int printAt_Y, const Color color);
void update_cursor_render();
void cursor_backspace();
void delete_unprotected_chars();


// --- LINE MGT ---
void newline();
void prevline();
void handle_automatic_newline();
void handle_scroll();


// --- DRAW CHAR ---
void draw_char(const char c, const int printAt_X, const int printAt_Y, const Color color, const bool interactable);


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
void redraw();


// --- INIT ---
void init_print();


#endif // PRINT_H
