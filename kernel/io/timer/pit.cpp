#include "config.h"
#include "io.h"

void pit_init() {
    outb(0x43, 0x36); // tell PIT channel 0, expect 2-byte divisor, fire repeatedly
    outb(0x40, (1193182 / PIT_TICK_HZ) & 0xFF); // divisor low byte
    outb(0x40, (1193182 / PIT_TICK_HZ) >> 8); // divisor high byte
}
