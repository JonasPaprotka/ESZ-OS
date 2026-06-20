#ifndef LIMINE_BOOT_H
#define LIMINE_BOOT_H

#include <stdint.h>
#include "limine.h"

struct BootInfo {
    volatile uint32_t* framebuffer;
    uint64_t width;
    uint64_t height;
    uint64_t pitch; // bytes per line
    uint16_t bpp; // bits per pixel
};

extern BootInfo boot_info;

volatile extern limine_framebuffer_request framebuffer_request;
volatile extern limine_memmap_request memmap_request;
volatile extern limine_hhdm_request hhdm_request;
volatile extern limine_rsdp_request rsdp_request;

#endif // LIMINE_BOOT_H
