#include "limine_boot.h"
#include "print.h"
#include "screenBuffer.h"

void clearScreen() {
    if (useScreenBuffer) {
        init_empty_screen_buffer();
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
    
    const int x = printAt_X * FONT_W;
    const int y = renderY * FONT_H;

    const int width = calc_line_pixels();

    for (int row = 0; row < FONT_H; row++) {
        for (int col = 0; col < FONT_W; col++) {
            boot_info.framebuffer[(y + row) * width + (x + col)] = 0;
        }
    }
}
