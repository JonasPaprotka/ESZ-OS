#pragma once

#include <stdint.h>

extern volatile uint64_t ticks;

extern "C" void timer_handler();

uint64_t get_ticks_in_ms();

void sleep_sec(const uint64_t sec);
void sleep_ms(const uint64_t ms);
