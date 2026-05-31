#include "terminal.h"
#include "io/keyboard.h"
#include "string.h"
#include "print.h"
#include "commands.h"
#include "maps/keymap.h"
#include "helper/info_text.h"
#include "shell/commands.h"

int lineInputLength = 0;
char lineInputBuffer[256];

char commandHistory[100][256];
unsigned int cmdHistCount = 0;
unsigned int goThroughHistoryCount = 0;

void add_command_to_history(char command[256]) {
    if (cmdHistCount >= 100) {
        for (int i = 0; i < cmdHistCount - 1; i++) {
            str_copy(commandHistory[i], commandHistory[i + 1]);
        }
        commandHistory[cmdHistCount - 1][0] = 0;
        cmdHistCount--;
    }
    
    str_copy(commandHistory[cmdHistCount], command);
    cmdHistCount++;
}

void handle_show_history() {
    delete_unprotected_chars();
    str_copy(lineInputBuffer, commandHistory[cmdHistCount - goThroughHistoryCount]);
    lineInputLength = str_length(lineInputBuffer);
    print_chars(lineInputBuffer, true);
}

void printHeader() {
    printSeperator();
    printInfoLine(InfoTextType::Success, "Kernel Loaded");
    printInfoLine(InfoTextType::Warning, "Project W.I.P.");
    print("ESZ-OS (64bit)");
    print("by Jonas Paprotka");
    printSeperator();
}

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

    add_command_to_history(lineInputBuffer);

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
    printInfoLine(InfoTextType::Error, String("Unknown Command: '", lineInputBuffer, "'"));
    lineInputLength = 0;
    lineInputBuffer[0] = 0; // clear buffer
}

void newTerminalInputLine() {
    const char* linePrefix = "esz >> ";
    print_inline(linePrefix);
}

void terminal_on_key(const unsigned char scancode) {
    uint16_t key = scancode_to_keycode(scancode);
    
    if (isExtendedScancode) {
        switch(scancode) {
            case 0x48: // ARROW UP
                if (cmdHistCount == 0) break;
                
                if (goThroughHistoryCount < cmdHistCount) {
                    goThroughHistoryCount++;   
                } else {
                    break;
                }
                
                handle_show_history();
                break;
            case 0x50: // ARROW DOWN
                if (goThroughHistoryCount == 0) break;
                goThroughHistoryCount--;
                handle_show_history();
                break;
            case 0x4B: // ARROW LEFT
                break;
            case 0x4D: // ARROW RIGHT
                break;
        }

        isExtendedScancode = false;
        return;
    }

    switch(key) {
        case KeyCode::KEY_ENTER:
            goThroughHistoryCount = 0; // for arrow cmd history - default: 1
            processLineInputBuffer();
            newTerminalInputLine();
            break;
        case KeyCode::KEY_BACKSPACE:
            if (lineInputLength == 0) break;
            cursor_backspace();
            --lineInputLength;
            lineInputBuffer[lineInputLength] = 0;
            break;
        case KeyCode::KEY_TAB:
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
    printInfoLine(InfoTextType::Loading, "Initializing Terminal...");
        
    printHeader();
    newTerminalInputLine();
}
