#include "vga.h"
#include "terminal.h"
#include "io.h"
#include "string.h"

void scrollDown() {
    // shift lines and clear last one
    unsigned char *video_memory = (unsigned char *) 0xB8000;
    for (int i = 1; i <= 3840; i++) {
        video_memory[i - 1] = video_memory[i + 159];
    }
    for (int i = 1920; i <= 1999; i++) {
        clear_char(i);
    }
    cursorAtChar = 0;
    cursorAtLine = 24;
    set_cursor(1920);
}

bool handleScroll() {
    if (cursorAtLine >= 25) {
        scrollDown();
        return true;
    }
    return false;
}

void newline() {
    cursorAtLine++;
    cursorAtChar = 0;
    handleScroll();
}

//region Print
void handleCursorUpdateOnPrint(int charAtPos) {
    cursorAtChar++;
    if (cursorAtChar >= 80) {
        newline();
        return;
    }
    if (not handleScroll()) {
        set_cursor(charAtPos + 1);
    }
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

void print_text(const string text, enum Color textColor) {
    for (int i = 0; text[i] != 0; i++) {
        print_char(text[i], textColor);
    }
}

void print(const string text, enum Color textColor) {
    print_text(text, textColor);
    newline();
}

void print(const string text) {
    print_text(text, White);
    newline();
}

void print_inline(const string text, enum Color textColor) {
    print_text(text, textColor);
}

void print_inline(const string text) {
    print_text(text, White);
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

void cursor_backspace() {
    --cursorAtChar;
    clear_char(80 * cursorAtLine + cursorAtChar);
}
