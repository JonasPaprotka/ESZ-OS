#include "screenBuffer.h"
#include "config.h"
#include "print.h"
#include "memory.h"
#include "heap.h"

ScreenBuffer* screenBufferPtr;

Line* get_screen_buffer_line(const int y) {
    return &screenBufferPtr->lines[y];
}

Cell* get_screen_buffer_cell(const int y, const int x) {
    return &screenBufferPtr->lines[y].cells[x];
}

Cell* get_screen_buffer_cell(const Line* line, const int x) {
    return &line->cells[x];
}

bool init_empty_screen_buffer() {
    const unsigned int storedRenderLineHistory = SCROLL_HISTORY_LINES;

    if (screenBufferPtr) {
        free(screenBufferPtr);
        screenBufferPtr = nullptr;
    }

    screenBufferPtr = (ScreenBuffer*) malloc(
        sizeof(ScreenBuffer) +
        (sizeof(Line) * (MAX_LINES + storedRenderLineHistory)) +
        (sizeof(Cell) * (MAX_LINES + storedRenderLineHistory) * MAX_CHARS)
    );

    if (screenBufferPtr == nullptr) return false;

    screenBufferPtr->lines = (Line*) (screenBufferPtr + 1);
    screenBufferPtr->amountOfLines = cursorAt_Y;
    screenBufferPtr->visibleLines = MAX_LINES;
    screenBufferPtr->startRenderLine = 0;
    screenBufferPtr->maxStoredLines = MAX_LINES + storedRenderLineHistory;


    const int allLines = screenBufferPtr->maxStoredLines;
    Cell* cellBlock = (Cell*)(screenBufferPtr->lines + allLines);

    memory_clear(cellBlock, sizeof(Cell) * allLines * MAX_CHARS);

    for (int i = 0; i < allLines; i++) {
        screenBufferPtr->lines[i].cells = cellBlock + i * MAX_CHARS;
        screenBufferPtr->lines[i].amountOfCells = 0;
    }

    return true;
}

bool init_screen_buffer() {
    if (!init_empty_screen_buffer()) return false;
    useScreenBuffer = true;

    cursorRendered_X = cursorAt_X;
    cursorRendered_Y = cursorAt_Y;

    return true;
}
