#include "limine_boot.h"

#define FONT8x16_IMPLEMENTATION

#include "font8x16.h"
#include "print.h"
#include "string.h"
#include "color.h"
#include <stdint.h>

int cursorAt_X;
int cursorAt_Y;

static constexpr int FONT_SIZE = 4;
static constexpr int FONT_W = 8 * FONT_SIZE;
static constexpr int FONT_H = 16 * FONT_SIZE;

unsigned int MAX_CHARS;

bool cursor_visible = true;
const char cursor_view = '_';


void newline() {
    cursorAt_X = 0;
    cursorAt_Y++;
}

void handle_automatic_newline() {
    if (cursorAt_X >= MAX_CHARS) newline();
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


void init_print() {
    MAX_CHARS = boot_info.width / FONT_W;
}


// LEGACY
/* void scrollDown() {
    // shift lines and clear last one
    byte *video_memory = (byte *) 0xB8000;
    for (int i = 1; i <= 3840; i++) {
        video_memory[i - 1] = video_memory[i + 159];
    }
    for (int i = 1920; i <= 1999; i++) {
        clear_char(i);
    }
    cursorAtChar = 0;
    cursorAtLine = 24;
    set_cursor(1920);
}
 */

/*  void cursor_backspace() {
    --cursorAtChar;
    clear_char(80 * cursorAtLine + cursorAtChar);
}
 */