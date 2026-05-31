#include "limine_boot.h"
#include "config.h"

#define FONT8x16_IMPLEMENTATION

#include "font8x16.h"
#include "print.h"
#include "string.h"
#include "color.h"
#include <stdint.h>
#include "clear.h"
#include "screenBuffer.h"
#include "math.h"

int cursorAt_X;
int cursorAt_Y;

int FONT_W;
int FONT_H;

int MAX_CHARS;
int MAX_LINES;

bool useScreenBuffer;
bool isRedrawing;


void newline() {
    if (useScreenBuffer && !isRedrawing) {
        screenBufferPtr->amountOfLines++;
    }

    cursorAt_X = 0;
    cursorAt_Y++;
    handle_scroll();
}

void prevline() {
    if (useScreenBuffer && !isRedrawing) {
        screenBufferPtr->amountOfLines--;
    }

    cursorAt_Y--;
    handle_scroll();
}

void handle_automatic_newline() {
    if (cursorAt_X >= MAX_CHARS) newline();
}

void cursor_backspace() {
    if (!screenBufferPtr) return;

    //TODO fix 0-1 bug and support newline backspace
    if (screenBufferPtr->lines[cursorAt_Y].cells[cursorAt_X - 1].interactable == false) return;
 
    if (cursorAt_X == 0) {
        if (cursorAt_Y > 0) {
            cursorAt_Y--;
            cursorAt_X = MAX_CHARS - 1;
        } else return;
    } else cursorAt_X--;

    screenBufferPtr->lines[cursorAt_Y].cells[cursorAt_X].text = 0;
    screenBufferPtr->lines[cursorAt_Y].amountOfCells--;

    clear_char(cursorAt_X, cursorAt_Y);
}

void delete_unprotected_chars() {
    const int length = screenBufferPtr->lines[cursorAt_Y].amountOfCells;

    for (int i = 0; i < length; i++) {
        cursor_backspace();
    }    
}

uint64_t calc_line_pixels() {
    return boot_info.pitch / 4;
}

void draw_char(char c, const int printAt_X, const int printAt_Y, Color color, bool interactable) {
    if (useScreenBuffer && !isRedrawing) {
        screenBufferPtr->lines[cursorAt_Y].cells[cursorAt_X].text = c;
        screenBufferPtr->lines[cursorAt_Y].cells[cursorAt_X].color = color;
        screenBufferPtr->lines[cursorAt_Y].cells[cursorAt_X].interactable = interactable;

        screenBufferPtr->lines[cursorAt_Y].amountOfCells++;
    }

    int renderY = printAt_Y;
    if (useScreenBuffer) renderY -= (int) screenBufferPtr->startRenderLine;

    // 8 x 16 arr char rendering
    const int x = printAt_X * FONT_W;
    const int y = renderY * FONT_H;

    const int width = calc_line_pixels();

    for (int row = 0; row < FONT_H; row++) {
        uint8_t bits = font8x16[(unsigned char) c][row / FONT_SIZE];
        
        for (int col = 0; col < FONT_W; col++) {
            boot_info.framebuffer[(y + row) * width + (x + col)] = (bits & (0x80 >> (col / FONT_SIZE))) ? (uint32_t) color : 0;
        }
    }
}

void print_char(const char c) {
    draw_char(c, cursorAt_X, cursorAt_Y, Color::White, true);
    cursorAt_X++;
    handle_automatic_newline();
}

void print_chars(const char* text, bool interactable) {
    const int s_len = str_length(text);
    for (int i = 0; i < s_len; i++) {
        draw_char(text[i], cursorAt_X, cursorAt_Y, Color::White, interactable);
        cursorAt_X++;
        handle_automatic_newline();
    }
}

void print_chars(const char* text, Color color) {
    const int s_len = str_length(text);
    for (int i = 0; i < s_len; i++) {
        draw_char(text[i], cursorAt_X, cursorAt_Y, color, false);
        cursorAt_X++;
        handle_automatic_newline();
    }
}

void redraw_char(const char c, Color color, bool interactable) {
    draw_char(' ', cursorAt_X, cursorAt_Y, color, interactable);
    draw_char(c, cursorAt_X, cursorAt_Y, color, interactable);
    cursorAt_X++;
    handle_automatic_newline();
}

void redraw_line(const int line) {
    const int cellAmount = screenBufferPtr->lines[line].amountOfCells;
    cursorAt_X = 0;
    cursorAt_Y = line;

    for (int i = 0; i < MAX_CHARS; i++) {
        if (i < cellAmount) {
            Cell& cell = screenBufferPtr->lines[line].cells[i];
            draw_char(cell.text, cursorAt_X, cursorAt_Y, cell.color, cell.interactable);
        } else {
            draw_char(' ', cursorAt_X, cursorAt_Y, Color::White, false);
        }
        cursorAt_X++;
    }
}

void redraw() {
    isRedrawing = true;
    const int savedX = cursorAt_X;
    const int savedY = cursorAt_Y;

    const int lineAmount = screenBufferPtr->visibleLines;
    const int renderStart = screenBufferPtr->startRenderLine;

    for (int i = 0; i < lineAmount; i++) {
        redraw_line(renderStart + i);
    }

    cursorAt_X = savedX;
    cursorAt_Y = savedY;
    isRedrawing = false;
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

void handle_scroll() {
    if (!useScreenBuffer || isRedrawing) return;

    // HANDLE dropping old history
    const int maxLines = screenBufferPtr->maxStoredLines;
    if (cursorAt_Y >= maxLines) {
        for (int line = 0; line < maxLines - 1; line++) {
            const int nextLine = line + 1;
            const int cellAmount = screenBufferPtr->lines[nextLine].amountOfCells;
            screenBufferPtr->lines[line].amountOfCells = cellAmount;
            
            for (int cell = 0; cell < cellAmount; cell++) {
                screenBufferPtr->lines[line].cells[cell].text = screenBufferPtr->lines[nextLine].cells[cell].text;
                screenBufferPtr->lines[line].cells[cell].color = screenBufferPtr->lines[nextLine].cells[cell].color;
                screenBufferPtr->lines[line].cells[cell].interactable = screenBufferPtr->lines[nextLine].cells[cell].interactable;
            }
        }

        screenBufferPtr->amountOfLines--;
        if (screenBufferPtr->startRenderLine > 0) screenBufferPtr->startRenderLine--;
        cursorAt_Y--;
        screenBufferPtr->lines[cursorAt_Y].amountOfCells = 0;
    }

    // SHIFT lines up
    if (cursorAt_Y >= (int) (screenBufferPtr->startRenderLine + screenBufferPtr->visibleLines)) {   
        screenBufferPtr->startRenderLine++;
        redraw();
    }
}

// INIT
void init_print() {
    useScreenBuffer = false;

    FONT_W = 8 * FONT_SIZE;
    FONT_H = 16 * FONT_SIZE;

    MAX_CHARS = calc_line_pixels() / FONT_W;
    MAX_LINES = boot_info.height / FONT_H;
}
