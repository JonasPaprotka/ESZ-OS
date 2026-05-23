#include "terminal.h"
#include "vga.h"
#include "keyboard.h"
#include "string.h"
#include "commands.h"

char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

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
    print("Unknown command", Red);
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
            char c = scancode_to_ascii[scancode];
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
