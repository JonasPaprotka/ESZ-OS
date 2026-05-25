#include "terminal.h"
#include "vga.h"
#include "keyboard.h"
#include "string.h"
#include "commands.h"
#include "keymap.h"
#include "byte.h"
#include "info_text.h"

const KeyboardLayout current_layout = LAYOUT_DE;

char scancode_to_char(const byte scancode) {
    if (scancode > 0x39) return 0;  // outside of table range
    
    if (current_layout == LAYOUT_DE) {
        return shift ? de_shifted[scancode] : de_unshifted[scancode];
    }
    return shift ? us_shifted[scancode] : us_unshifted[scancode];
}

void printHeader() {
    printSeperator();
    printInfoLine(Success, "Kernel Loaded");
    printInfoLine(Warning, "Project W.I.P.");
    print("ESZ-OS (32bit)");
    print("by Jonas Paprotka");
    printSeperator();
}

int cursorAtChar = 0;
int cursorAtLine = 0;
int charsProtectedTil = 0;
int lineFullLength = 0;

int lineInputLength = 0;
char lineInputBuffer[256];

void processLineInputBuffer() {
    if (lineInputLength == 0) {
        newline();
        lineInputBuffer[0] = 0;
        return;
    }

    if (lineInputLength > 256) {
        lineInputLength = 0;
        lineInputBuffer[0] = 0;
        newline();
        printInfoLine(Error, "Command exceeds 256 chars");
        return;
    }

    const char* args = "";
    for (int i = 0; i < lineInputLength; i++) {
        if (lineInputBuffer[i] == ' ') {
            lineInputBuffer[i] = 0;
            args = &lineInputBuffer[i+1];
            break;
        }
    }

    for (int i = 0; commands[i].name != 0; i++) {
        if (str_equal(lineInputBuffer, commands[i].name)) {
            newline();
            commands[i].execute(args);
            lineInputLength = 0;
            lineInputBuffer[0] = 0;
            return;
        }
    }

    newline();
    printInfoLine(Error, "Unknown Command");
    lineInputLength = 0;
    lineInputBuffer[0] = 0; // clear buffer
}

void newTerminalInputLine() {
    cstr linePrefix = "esz >> ";
    print_inline(linePrefix);
    cursorAtChar = str_length(linePrefix);
    charsProtectedTil = cursorAtChar;
}

void terminal_on_key(const byte scancode) {
    switch(scancode) {
        case 0x1C: // Enter
            processLineInputBuffer();
            newTerminalInputLine();
            break;
        case 0x0E: // Backspace
            if (cursorAtChar <= charsProtectedTil) { break; }
            cursor_backspace();
            --lineInputLength;
            lineInputBuffer[lineInputLength] = 0;
            break;
        default:
            char c = scancode_to_char(scancode);
            if (!c) return;
            print_char(c);

            lineInputBuffer[lineInputLength] = c;
            ++lineInputLength;
            lineInputBuffer[lineInputLength] = 0; // null-terminate
            break;
    }
}

void terminal_init() {
    printHeader();
    newTerminalInputLine();
}
