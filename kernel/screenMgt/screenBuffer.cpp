#include "screenMgt/screenBuffer.h"
#include "memory.h"
#include "print.h"
#include "info_text.h"

ScreenBuffer* screenBufferPtr;


void init_screen_buffer() {
    printInfoLine(InfoTextType::Loading, "Loading Screen Buffer...");

    const int storedRenderLineHistory = 100;

    screenBufferPtr = (ScreenBuffer*) malloc(
        sizeof(ScreenBuffer) +
        (sizeof(Line) * (MAX_LINES + storedRenderLineHistory)) +
        (sizeof(Cell) * (MAX_LINES + storedRenderLineHistory) * MAX_CHARS)
    );

    screenBufferPtr->lines = (Line*) (screenBufferPtr + 1);
    screenBufferPtr->amountOfLines = cursorAt_Y;
    screenBufferPtr->visibleLines = MAX_LINES;
    screenBufferPtr->startRenderLine = 0;
    screenBufferPtr->maxStoredLines = MAX_LINES + storedRenderLineHistory;


    const int allLines = screenBufferPtr->maxStoredLines;
    Cell* cellBlock = (Cell*)(screenBufferPtr->lines + allLines);

    for (int i = 0; i < allLines; i++) {
        screenBufferPtr->lines[i].cells = cellBlock + i * MAX_CHARS;
        screenBufferPtr->lines[i].amountOfCells = 0;
    }

    useScreenBuffer = true;
}
