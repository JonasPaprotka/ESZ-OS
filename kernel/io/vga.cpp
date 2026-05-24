#include "vga.h"
#include "terminal.h"
#include "io.h"
#include "string.h"

//region Print
void handleCursorUpdateOnPrint(int charAtPos) {
    cursorAtChar++;
    if (cursorAtChar >= 80) {
        cursorAtChar = 0;
        cursorAtLine++;
    }
    if (cursorAtLine >= 25) {
        //TODO implement scrolling somehow
    }

    set_cursor(charAtPos + 1);
}

void print_char(char c, enum Color charColor) {
    unsigned char *video_memory = (unsigned char *) 0xB8000;
    int charAtPos = 80 * cursorAtLine + cursorAtChar;
    video_memory[charAtPos * 2] = c;
    video_memory[charAtPos * 2 + 1] = charColor;
    handleCursorUpdateOnPrint(charAtPos);
}

void print_char(char c) {
    unsigned char *video_memory = (unsigned char *) 0xB8000;
    int charAtPos = 80 * cursorAtLine + cursorAtChar;
    video_memory[charAtPos * 2] = c;
    video_memory[charAtPos * 2 + 1 ] = White;
    handleCursorUpdateOnPrint(charAtPos);
}

void print(const string text, enum Color textColor, int lineNo, int charPadding) {
    for (int i = 0; text[i] != 0; i++) {
        print_char(text[i], textColor);
    }
}

void print(const string text, enum Color textColor) {
    print(text, textColor, cursorAtLine, cursorAtChar);
    cursorAtLine++;
    cursorAtChar = 0;
}

void print(const string text) {
    print(text, White, cursorAtLine, cursorAtChar);
    cursorAtLine++;
    cursorAtChar = 0;
}

void print_inline(const string text, enum Color textColor) {
    print(text, textColor, cursorAtLine, cursorAtChar);
}

void print_inline(const string text) {
    print(text, White, cursorAtLine, cursorAtChar);
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
