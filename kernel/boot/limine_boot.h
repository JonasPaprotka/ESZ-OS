#ifndef LIMINE_BOOT_H
#define LIMINE_BOOT_H

#include <stdint.h>

struct BootInfo {
    volatile uint32_t* framebuffer;
    uint64_t width;
    uint64_t height;
    uint64_t pitch; // bytes per line
    uint16_t bpp; // bits per pixel
};

extern BootInfo boot_info;

#endif
