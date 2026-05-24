#ifndef IO_H
#define IO_H
#include "byte.h"

void outb(unsigned short port, byte value);
byte inb(unsigned short port);

#endif
