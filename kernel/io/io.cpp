
void outb(const unsigned short port, const unsigned char value) {
    __asm__("outb %0, %1" : : "a"(value), "Nd"(port));
}

unsigned char inb(unsigned short port) {
    unsigned char value;
    __asm__("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}
