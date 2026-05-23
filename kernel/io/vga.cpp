#include "vga.h"
#include "terminal.h"
#include "io.h"
#include "string.h"

//region Print
void print_char(char c, enum Color charColor, int charLocation) {
    unsigned char *video_memory = (unsigned char *) 0xB8000;
    video_memory[charLocation * 2] = c;
    video_memory[charLocation * 2 + 1] = charColor;
    set_cursor(charLocation + 1);
}

void print_char(char c) {
    unsigned char *video_memory = (unsigned char *) 0xB8000;
    video_memory[(80 * cursorAtLine + cursorAtChar) * 2] = c;
    video_memory[(80 * cursorAtLine + cursorAtChar) * 2 + 1 ] = White;
    set_cursor(80 * cursorAtLine + cursorAtChar + 1);
}

void print(const string text, enum Color textColor, int lineNo, int charPadding) {
    for (int i = 0; text[i] != 0; i++) {
        print_char(text[i], textColor, 80 * lineNo + i + charPadding);
    }
}

void print(const string text, enum Color textColor) {
    print(text, textColor, cursorAtLine, cursorAtChar);
    ++cursorAtLine;
    cursorAtChar = 0;
}

void print(const string text) {
    print(text, White, cursorAtLine, cursorAtChar);
    ++cursorAtLine;
    cursorAtChar = 0;
}
//endregion Print

void clear() {
    int chars = 2000;
    for (int i = 0; i <= chars; i++) {
        clear_char(i);
    }
    cursorAtChar = 0;
    cursorAtLine = 0;
    set_cursor(0);
}

void clear_char(int pos) {
    unsigned char *video_memory = (unsigned char *) 0xB8000;
    video_memory[pos * 2] = ' ';
    video_memory[pos * 2 + 1] = White;
    set_cursor(pos);
}

void set_cursor(int pos) {
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
    outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8) & 0xFF);
}
