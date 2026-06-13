#include "config.h"
#include "terminal.h"
#include "io/keyboard.h"
#include "string.h"
#include "print.h"
#include "commands.h"
#include "maps/keymap.h"
#include "helper/info_text.h"
#include "shell/commands.h"
#include "args.h"

uint64_t lineInputLength = 0;
char lineInputBuffer[TERMINAL_BUFFER_SIZE];

char commandHistory[MAX_COMMAND_HISTORY][TERMINAL_BUFFER_SIZE];
uint64_t cmdHistCount = 0;
uint64_t goThroughHistoryCount = 0;

void add_command_to_history(char command[TERMINAL_BUFFER_SIZE]) {
    if (cmdHistCount >= MAX_COMMAND_HISTORY) {
        for (uint64_t i = 0; i < cmdHistCount - 1; i++) {
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
    newline();
    print("#####  #####  #####");
    print("#      #         ##");
    print("####   #####   ##  ");
    print("#          #  ##   ");
    print("#####  #####  #####");
    newline();
    cmd_sysinfo();
    printSeperator();
}

void displayTerminalError(const char* Text) {
    newline();
    printInfoLine(InfoTextType::Error, Text);
    lineInputLength = 0;
    lineInputBuffer[0] = 0; // clear buffer
}

void processLineInputBuffer() {
    if (lineInputLength == 0) {
        newline();
        lineInputBuffer[0] = 0;
        return;
    }

    if (lineInputLength > TERMINAL_BUFFER_SIZE) {
        displayTerminalError(String("Command exceeds ", TERMINAL_BUFFER_SIZE, " chars"));
        return;
    }

    add_command_to_history(lineInputBuffer);

    const char* args = getInputArgs(lineInputLength, lineInputBuffer);
    if (!executeCommand(lineInputLength, lineInputBuffer, args)) {
        displayTerminalError(String("Unknown Command: '", lineInputBuffer, "'"));
        return;
    }
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
