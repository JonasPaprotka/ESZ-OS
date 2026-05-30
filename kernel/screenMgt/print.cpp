#include "limine_boot.h"

#define FONT8x16_IMPLEMENTATION

#include "font8x16.h"
#include "print.h"
#include "string.h"
#include "color.h"
#include <stdint.h>
#include "clear.h"

int FONT_SIZE;

int cursorAt_X;
int cursorAt_Y;

int FONT_W;
int FONT_H;

unsigned int MAX_CHARS;

bool cursor_visible = true;
const char cursor_view = '_';

void scroll() {
    //TODO
}

void handleScroll() {
    //TODO
}

void newline() {
    cursorAt_X = 0;
    cursorAt_Y++;
    handleScroll();
}

void prevline() {
    cursorAt_Y--;
    handleScroll();
}

void handle_automatic_newline() {
    if (cursorAt_X >= MAX_CHARS) newline();
}

void cursor_backspace() {
    if (cursorAt_X == 0) {
        if (cursorAt_Y > 0) {
            cursorAt_Y--;
            cursorAt_X = MAX_CHARS - 1;
        } else return;
    } else cursorAt_X--;

    clear_char(cursorAt_X, cursorAt_Y);
}

void draw_char(char c, const int printAt_X, const int printAt_Y, Color color) {
    // 8 x 16 arr char rendering
    int x = printAt_X * FONT_W;
    int y = printAt_Y * FONT_H;

    for (int row = 0; row < FONT_H; row++) {
        uint8_t bits = font8x16[(unsigned char) c][row / FONT_SIZE];
        
        for (int col = 0; col < FONT_W; col++) {
            if (bits & (0x80 >> (col / FONT_SIZE))) {
                boot_info.framebuffer[(y + row) * boot_info.width + (x + col)] = (unsigned int) color;
            }
        }
    }
}

void print_char(const char c) {
        draw_char(c, cursorAt_X, cursorAt_Y, Color::White);
        cursorAt_X++;
        handle_automatic_newline();
}

void print_chars(const char* text, Color color) {
    const int s_len = str_length(text);
    for (int i = 0; i < s_len; i++) {
        draw_char(text[i], cursorAt_X, cursorAt_Y, color);
        cursorAt_X++;
        handle_automatic_newline();
    }
}

void print_inline(const char* text, Color color) {
    print_chars(text, color);
}

void print_inline(const char* text) {
    print_chars(text, Color::White);
}

void print(const char* text, Color color) {
    print_chars(text, color);
    newline();
}

void print(const char* text) {
    print_chars(text, Color::White);
    newline();
}

// INIT
void init_print() {
    FONT_SIZE = 2;
    FONT_W = 8 * FONT_SIZE;
    FONT_H = 16 * FONT_SIZE;

    MAX_CHARS = boot_info.width / FONT_W;
}
