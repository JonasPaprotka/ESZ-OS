#include "vga.h"
#include "terminal.h"
#include "io.h"

void print_char(char toBePrintedCharacter, enum Color charColor, int charLocation) {
    unsigned char *video_memory = (unsigned char *) 0xB8000;
    video_memory[charLocation * 2] = toBePrintedCharacter;
    video_memory[charLocation * 2 + 1] = charColor;
    set_cursor(charLocation);
}

void printf(const char* text, enum Color textColor, int lineNo, int charPadding) {
    for (int i = 0; text[i] != 0; i++) {
        print_char(text[i], textColor, 80 * lineNo + i + charPadding);
    }
}

void clear() {
    int chars = 2000;
    for (int i = 0; i <= chars; i++) {
        print_char(' ', Black, i);
    }
}

void set_cursor(int pos) {
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
    outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8) & 0xFF);
}
