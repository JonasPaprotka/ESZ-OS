#ifndef SCREEN_BUFFER_H
#define SCREEN_BUFFER_H

#include "color.h"

struct Cell {
    Color color;
    char text;
    bool interactable;
};

struct Line {
    Cell* cells;
    unsigned int amountOfCells;
} __attribute__((packed));

struct ScreenBuffer {
    Line* lines;
    unsigned int amountOfLines;
    unsigned int visibleLines;
    unsigned int startRenderLine;
    unsigned int maxStoredLines;
} __attribute__((packed));

extern ScreenBuffer* screenBufferPtr;

Line* get_screen_buffer_line(const int y);
Cell* get_screen_buffer_cell(const int y, const int x);
Cell* get_screen_buffer_cell(const Line* line, const int x);

bool is_prev_char_editable();
bool is_next_char_editable();

void init_screen_buffer();
void init_empty_screen_buffer();

#endif
