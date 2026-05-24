// File Updated with AI
#include "byte.h"

void outb(unsigned short port, byte value) {
    __asm__("outb %0, %1" : : "a"(value), "Nd"(port));
}

byte inb(unsigned short port) {
    byte value;
    __asm__("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}
