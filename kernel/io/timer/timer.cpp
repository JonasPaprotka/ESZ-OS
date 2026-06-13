#include "config.h"
#include <stdint.h>
#include "io.h"
#include "pic.h"

volatile uint64_t ticks = 0;

extern "C" void timer_handler() {
    ticks++;
    EIO_Finished_Interrupt();
}

uint64_t get_ticks_in_ms() {
    return ticks * 1000 / PIT_TICK_HZ;
}
