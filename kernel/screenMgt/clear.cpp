#include "limine_boot.h"
#include "print.h"
#include "screenBuffer.h"

void clearScreen() {
    if (useScreenBuffer) {
        screenBufferPtr->amountOfLines = 0;
        screenBufferPtr->startRenderLine = 0;
    }

    const int width = calc_line_pixels(); 
    const int heigth = boot_info.height;

    for (int y = 0; y < heigth; y++) {
        for (int x = 0; x < width; x++) {
            boot_info.framebuffer[y * width + x] = 0;
        }
    }
}

void clear_char(const int printAt_X, const int printAt_Y) {
    int renderY = printAt_Y;
    if (useScreenBuffer) renderY -= (int) screenBufferPtr->startRenderLine;
    
    int x = printAt_X * FONT_W;
    int y = renderY * FONT_H;

    const int width = calc_line_pixels();

    for (int row = 0; row < FONT_H; row++) {
        for (int col = 0; col < FONT_W; col++) {
            boot_info.framebuffer[(y + row) * width + (x + col)] = 0;
        }
    }
}
