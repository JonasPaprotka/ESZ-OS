#include "io.h"

// out-byte (8bit)
void outb(const uint16_t port, const uint8_t value) {
    __asm__("outb %0, %1" : : "a"(value), "Nd"(port));
}

// in-byte (8bit)
uint8_t inb(const uint16_t port) {
    uint8_t value;
    __asm__("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

// out-word (16bit)
void outw(const uint16_t port, const uint16_t value) {
    __asm__("outw %0, %1" : : "a"(value), "Nd"(port));
}

// in-word (16bit)
uint16_t inw(const uint16_t port) {
    uint16_t value;
    __asm__("inw %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

// out-long (32bit)
void outl(const uint16_t port, const uint32_t value) {
    __asm__("outl %0, %1" : : "a"(value), "Nd"(port));
}

// in-long (32bit)
uint32_t inl(const uint16_t port) {
    uint32_t value;
    __asm__("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}
