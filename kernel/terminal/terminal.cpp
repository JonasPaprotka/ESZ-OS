#include "terminal.h"
#include "io/keyboard.h"
#include "string.h"
#include "print.h"
#include "commands.h"
#include "maps/keymap.h"
#include "helper/info_text.h"
#include "shell/commands.h"

const KeyboardLayout current_layout = LAYOUT_DE;

char scancode_to_char(const unsigned char scancode) {
    if (scancode > 0x39) return 0;  // outside of table range
    
    if (current_layout == LAYOUT_DE) {
        return shift ? de_shifted[scancode] : de_unshifted[scancode];
    }
    return shift ? us_shifted[scancode] : us_unshifted[scancode];
}


void printHeader() {
    printSeperator();
    printInfoLine(InfoTextType::Success, "Kernel Loaded");
    printInfoLine(InfoTextType::Warning, "Project W.I.P.");
    print("ESZ-OS (64bit)");
    print("by Jonas Paprotka");
    printSeperator();
}

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
        printInfoLine(InfoTextType::Error, "Command exceeds 256 chars");
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
/*         if (str_equal(lineInputBuffer, commands[i].name)) {
            newline();
            commands[i].execute(args);
            lineInputLength = 0;
            lineInputBuffer[0] = 0;
            return;
        } */
    }

    newline();
    printInfoLine(InfoTextType::Error, "Unknown Command");
    lineInputLength = 0;
    lineInputBuffer[0] = 0; // clear buffer
}

void newTerminalInputLine() {
    const char* linePrefix = "esz >> ";
    print_inline(linePrefix);
}

void terminal_on_key(const unsigned char scancode) {
    switch(scancode) {
        case 0x1C: // Enter
            processLineInputBuffer();
            newTerminalInputLine();
            break;
        case 0x0E: // Backspace
            // if (cursorAt_X <= charsProtectedTil) { break; }
            // cursor_backspace();
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
