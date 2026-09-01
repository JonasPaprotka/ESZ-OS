#include "limine_boot.h"
#include <stdint.h>
#include <stddef.h>
#include "limine.h"

uint64_t hhdm_offset = 0;

// REQUESTS
__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(4);

__attribute__((used, section(".limine_requests")))
volatile struct limine_framebuffer_request framebuffer_request = {
    .id       = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0,
    .response = NULL
};

__attribute__((used, section(".limine_requests")))
volatile struct limine_memmap_request memmap_request = {
    .id       = LIMINE_MEMMAP_REQUEST_ID,
    .revision = 0,
    .response = NULL
};

__attribute__((used, section(".limine_requests")))
volatile struct limine_hhdm_request hhdm_request = {
    .id       = LIMINE_HHDM_REQUEST_ID,
    .revision = 0,
    .response = NULL
};

__attribute__((used, section(".limine_requests")))
volatile struct limine_rsdp_request rsdp_request = {
    .id       = LIMINE_RSDP_REQUEST_ID,
    .revision = 0,
    .response = NULL
};

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;


BootInfo boot_info = {};

static void hcf() {
    for (;;) { __asm__("hlt"); }
}

extern "C" void kernel_main();

// ENTRY
extern "C" void kmain() {
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) hcf();
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) hcf();

    limine_framebuffer* fb = framebuffer_request.response->framebuffers[0];

    boot_info.framebuffer = (volatile uint32_t*) fb->address;
    boot_info.width       = fb->width;
    boot_info.height      = fb->height;
    boot_info.pitch       = fb->pitch;
    boot_info.bpp         = fb->bpp;

    kernel_main();
    hcf();
}
