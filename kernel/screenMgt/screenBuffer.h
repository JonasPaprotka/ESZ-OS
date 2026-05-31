#ifndef SCREEN_BUFFER_H
#define SCREEN_BUFFER_H

#include "color.h"

void init_screen_buffer();
void init_empty_screen_buffer();

struct Cell {
    char text;
    Color color;
    bool interactable;
} __attribute__((packed));

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

#endif
