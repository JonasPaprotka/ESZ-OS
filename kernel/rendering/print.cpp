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
#include "terminal.h"

int cursorAt_X;
int cursorAt_Y;

int cursorRendered_X;
int cursorRendered_Y;

bool cursor_visible = false;

int FONT_W;
int FONT_H;

int ADJUSTED_WIDTH;

int MAX_CHARS;
int MAX_LINES;

bool useScreenBuffer;
bool isRedrawing;

void render_cursor(const int printAt_X, const int printAt_Y, const Color color) {
    const int x = printAt_X * FONT_W;
    const int y = (printAt_Y - (int) screenBufferPtr->startRenderLine) * FONT_H;

    for (int row = FONT_H - CURSOR_HEIGHT * FONT_SIZE; row < FONT_H; row++) {
        for (int col = 0; col < FONT_W; col++) {
            boot_info.framebuffer[(y + row) * ADJUSTED_WIDTH + (x + col)] = (uint32_t) color;
        }
    }
}

void update_cursor_render() {
    if (!useScreenBuffer) return;
    if (isRedrawing) return;

    Cell* cell = get_screen_buffer_cell(cursorRendered_Y, cursorRendered_X);

    isRedrawing = true;
    draw_char(
        cell->text,
        cursorRendered_X,
        cursorRendered_Y,
        cell->color,
        cell->interactable
    );

    if (cursor_visible) render_cursor(cursorAt_X, cursorAt_Y, Color::White);
    isRedrawing = false;

    cursorRendered_X = cursorAt_X;
    cursorRendered_Y = cursorAt_Y;
}

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
    if (cursorAt_X == (int) lineInputStart_X) return;

    Line* line = get_screen_buffer_line(cursorAt_Y);
    if (line->cells[cursorAt_X - 1].interactable == false) return;

    cursorAt_X--;
    lineInputCursorPos--;

    line->cells[cursorAt_X].text = 0;
    line->amountOfCells--;
    clear_char(cursorAt_X, cursorAt_Y);
    update_cursor_render();
}

void delete_unprotected_chars() {
    while (cursorAt_X > 0 && get_screen_buffer_cell(cursorAt_Y, cursorAt_X - 1)->interactable) {
        cursor_backspace();
    }
}

void draw_char(const char c, const int printAt_X, const int printAt_Y, const Color color, const bool interactable) {
    if (useScreenBuffer && !isRedrawing) {
        Line* line = get_screen_buffer_line(cursorAt_Y);
        Cell* cell = get_screen_buffer_cell(line, cursorAt_X);

        cell->text = c;
        cell->color = color;
        cell->interactable = interactable;

        line->amountOfCells++;
    }

    int renderY = printAt_Y;
    if (useScreenBuffer) renderY -= (int) screenBufferPtr->startRenderLine;

    // 8 x 16 arr char rendering
    const int x = printAt_X * FONT_W;
    const int y = renderY * FONT_H;

    for (int row = 0; row < FONT_H; row++) {
        uint8_t bits = font8x16[(uint8_t) c][row / FONT_SIZE];

        for (int col = 0; col < FONT_W; col++) {
            boot_info.framebuffer[(y + row) * ADJUSTED_WIDTH + (x + col)] = (bits & (0x80 >> (col / FONT_SIZE))) ? (uint32_t) color : 0;
        }
    }
}

void print_char(const char c) {
    draw_char(c, cursorAt_X, cursorAt_Y, Color::White, true);
    cursorAt_X++;
    update_cursor_render();
    handle_automatic_newline();
}

void print_char(const char c, const bool interactable) {
    draw_char(c, cursorAt_X, cursorAt_Y, Color::White, interactable);
    cursorAt_X++;
    update_cursor_render();
    handle_automatic_newline();
}

void print_chars(const char* text, const bool interactable) {
    const int s_len = str_length(text);
    for (int i = 0; i < s_len; i++) {
        draw_char(text[i], cursorAt_X, cursorAt_Y, Color::White, interactable);
        cursorAt_X++;
        update_cursor_render();
        handle_automatic_newline();
    }
}

void print_chars(const char* text, const Color color) {
    const int s_len = str_length(text);
    for (int i = 0; i < s_len; i++) {
        draw_char(text[i], cursorAt_X, cursorAt_Y, color, false);
        cursorAt_X++;
        update_cursor_render();
        handle_automatic_newline();
    }
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

    cursorRendered_X = cursorAt_X;
    cursorRendered_Y = cursorAt_Y;
}

void print_inline(const char* text, const Color color) {
    print_chars(text, color);
}

void print_inline(const char* text) {
    print_chars(text, Color::White);
}

void print(const char* text, const Color color) {
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
            Line* curr_buff_line = get_screen_buffer_line(line);
            Line* next_buff_line = get_screen_buffer_line(line + 1);

            const int cellAmount = next_buff_line->amountOfCells;
            curr_buff_line->amountOfCells = cellAmount;

            for (int cell = 0; cell < cellAmount; cell++) {
                Cell* curr_buff_cell = get_screen_buffer_cell(curr_buff_line, cell);
                Cell* next_line_buff_cell = get_screen_buffer_cell(next_buff_line, cell);

                curr_buff_cell->text = next_line_buff_cell->text;
                curr_buff_cell->color = next_line_buff_cell->color;
                curr_buff_cell->interactable = next_line_buff_cell->interactable;
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

    update_cursor_render();
}

// INIT
void init_print() {
    useScreenBuffer = false;

    FONT_W = 8 * FONT_SIZE;
    FONT_H = 16 * FONT_SIZE;

    ADJUSTED_WIDTH = boot_info.pitch / 4;
    MAX_CHARS = ADJUSTED_WIDTH / FONT_W;
    MAX_LINES = boot_info.height / FONT_H;
}
