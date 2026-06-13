#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

extern volatile uint64_t ticks;

extern "C" void timer_handler();

uint64_t get_ticks_in_ms();

#endif
