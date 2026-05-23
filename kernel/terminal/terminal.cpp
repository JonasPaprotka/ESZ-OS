#include "terminal.h"
#include "vga.h"
#include "keyboard.h"
#include "string.h"
#include "commands.h"
#include "keymap.h"

KeyboardLayout current_layout = LAYOUT_DE;
char scancode_to_char(unsigned char scancode) {
    if (scancode > 0x39) return 0;  // outside of table range
    
    if (current_layout == LAYOUT_DE) {
        return shift ? de_shifted[scancode] : de_unshifted[scancode];
    }
    return shift ? us_shifted[scancode] : us_unshifted[scancode];
}

void printHeader() {
    print("---------------------- ", DarkGray);
    print("[INFO]: Kernel Loaded", Green);
    print("[WARN]: Kernel W.I.P.", Yellow);
    print("ESZ-OS");
    print("by Jonas Paprotka");
    print("---------------------- ", DarkGray);
}

int cursorAtChar = 0;
int cursorAtLine = 0;
int charsProtectedTil = 0;

int lineImputLength = 0;
char lineInputBuffer[256];

void processLineInputBuffer() {
    if (lineImputLength == 0) {
        lineInputBuffer[0] = 0;
        return;
    }
    if (lineImputLength > 256) {
        lineImputLength = 0;
        lineInputBuffer[0] = 0;
        return;
    }

    const char* args = "";
    for (int i = 0; i < lineImputLength; i++) {
        if (lineInputBuffer[i] == ' ') {
            lineInputBuffer[i] = 0;
            args = &lineInputBuffer[i+1];
            break;
        }
    }

    for (int i = 0; commands[i].name != 0; i++) {
        if (strcmp(lineInputBuffer, commands[i].name)) {
            commands[i].execute(args);
            lineImputLength = 0;
            lineInputBuffer[0] = 0;
            return;
        }
    }

    ++cursorAtLine;
    cursorAtChar = 0;
    print("[ERROR]: Unknown command", Red);
    lineImputLength = 0;
    lineInputBuffer[0] = 0; // clear buffer
}

void newTerminalInputLine() {
    cursorAtChar = 0;
    string linePrefix = "root@esz >> ";
    print(linePrefix, White, cursorAtLine, 0);
    cursorAtChar = strlen(linePrefix);
    charsProtectedTil = cursorAtChar;
}

void terminal_on_key(unsigned char scancode) {
    switch(scancode) {
        case 0x1C: // Enter
            processLineInputBuffer();
            newTerminalInputLine();
            break;
        case 0x0E: // Backspace
            if (cursorAtChar == charsProtectedTil) { break; }
            --cursorAtChar;

            --lineImputLength;
            lineInputBuffer[lineImputLength] = 0;
            clear_char(80 * cursorAtLine + cursorAtChar);
            break;
        default:
            char c = scancode_to_char(scancode);
            if (!c) return;
            print_char(c);
            ++cursorAtChar;

            lineInputBuffer[lineImputLength] = c;
            ++lineImputLength;
            lineInputBuffer[lineImputLength] = 0; // null-terminate
            break;
    }
}

void terminal_init() {
    printHeader();
    newTerminalInputLine();
}
