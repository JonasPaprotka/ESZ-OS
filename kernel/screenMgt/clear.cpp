#include "limine_boot.h"
#include "print.h"

void clearScreen() {
    const int width = boot_info.width; 
    const int heigth = boot_info.height;

    for (int y = 0; y < heigth; y++) {
        for (int x = 0; x < width; x++) {
            boot_info.framebuffer[y * boot_info.width + x] = 0;
        }
    }
}

void clear_char(const int printAt_X, const int printAt_Y) {
    int x = printAt_X * FONT_W;
    int y = printAt_Y * FONT_H;

    for (int row = 0; row < FONT_H; row++) {
        for (int col = 0; col < FONT_W; col++) {
            boot_info.framebuffer[(y + row) * boot_info.width + (x + col)] = 0;
        }
    }
}
