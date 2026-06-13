#include "config.h"
#include <stdint.h>
#include "io.h"
#include "pic.h"

volatile uint64_t ticks = 0;

extern "C" void timer_handler() {
    ticks++;
    EIO_Finished_Interrupt();
}

const uint64_t get_ticks_in_ms() {
    return ticks * 1000 / PIT_TICK_HZ;
}

void halt_till_tick(const uint64_t targetTick) {
    while(ticks < targetTick) {
        __asm__ volatile("sti; hlt");
    }
}

void sleep_sec(const uint64_t sec) {
    halt_till_tick(ticks + PIT_TICK_HZ * sec);
}

void sleep_ms(const uint64_t ms) {
    halt_till_tick(ticks + ms * PIT_TICK_HZ / 1000);
}
