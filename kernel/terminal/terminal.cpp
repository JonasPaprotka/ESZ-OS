#include "terminal.h"
#include "io/keyboard.h"
#include "string.h"
#include "print.h"
#include "commands.h"
#include "maps/keymap.h"
#include "helper/info_text.h"
#include "shell/commands.h"

const KeyboardLayout current_layout = LAYOUT_DE;

uint16_t scancode_to_keycode(const unsigned char scancode) {
    if (scancode >= 0x3A) return KEY_UNKNOWN;
    int shift_idx = shift ? 1 : 0;

    if (current_layout == LAYOUT_DE) {
        return de_keymap[shift_idx][scancode];
    }

    return us_keymap[shift_idx][scancode];
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
        if (str_equal(lineInputBuffer, commands[i].name)) {
            newline();
            commands[i].execute(args);
            lineInputLength = 0;
            lineInputBuffer[0] = 0;
            return;
        }
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
    uint16_t key = scancode_to_keycode(scancode);

    switch(key) {
        case KeyCode::KEY_ENTER: // Enter
            processLineInputBuffer();
            newTerminalInputLine();
            break;
        case KeyCode::KEY_BACKSPACE: // Backspace
            // if (cursorAt_X <= charsProtectedTil) { break; }
            cursor_backspace();
            --lineInputLength;
            lineInputBuffer[lineInputLength] = 0;
            break;
        //TODO:
        // - cmd + "+" / "-" to change font size (needs storing of lines first!)
        // - Arrow key cursor movement right/left; up/down for prev/next command (also needs storage)
        // - TAB to complete command/paths etc

        default:
            char c = scancode_to_keycode(scancode);
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
