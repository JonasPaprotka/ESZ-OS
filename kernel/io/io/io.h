#ifndef IO_H
#define IO_H

#include <stdint.h>

void outb(const uint16_t port, const uint8_t value);
uint8_t inb(const uint16_t port);

void outw(const uint16_t port, const uint16_t value);
uint16_t inw(const uint16_t port);

void outl(const uint16_t port, const uint32_t value);
uint32_t inl(const uint16_t port);

#endif
