#include "limine_boot.h"
#include "print.h"
#include "screenBuffer.h"

void clearScreen() {
    if (useScreenBuffer) {
        init_empty_screen_buffer();
    }

    const int height = boot_info.height;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < ADJUSTED_WIDTH; x++) {
            boot_info.framebuffer[y * ADJUSTED_WIDTH + x] = 0;
        }
    }
}

void clear_char(const int printAt_X, const int printAt_Y) {
    int renderY = printAt_Y;
    if (useScreenBuffer) renderY -= (int) screenBufferPtr->startRenderLine;
    
    const int x = printAt_X * FONT_W;
    const int y = renderY * FONT_H;

    for (int row = 0; row < FONT_H; row++) {
        for (int col = 0; col < FONT_W; col++) {
            boot_info.framebuffer[(y + row) * ADJUSTED_WIDTH + (x + col)] = 0;
        }
    }
}
