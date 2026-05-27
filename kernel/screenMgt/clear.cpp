#include "limine_boot.h"

void clearScreen() {
    const int width = boot_info.width; 
    const int heigth = boot_info.height;

    for (int y = 0; y < heigth; y++) {
        for (int x = 0; x < width; x++) {
            boot_info.framebuffer[y * boot_info.width + x] = 0;
        }
    }
}
