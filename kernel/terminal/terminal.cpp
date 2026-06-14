#include "config.h"
#include "terminal.h"
#include "keyboard.h"
#include "string.h"
#include "print.h"
#include "commands.h"
#include "keymap.h"
#include "info_text.h"
#include "commands.h"
#include "args.h"
#include "screenBuffer.h"

uint64_t lineInputLength = 0;
char lineInputBuffer[TERMINAL_BUFFER_SIZE];
unsigned int lineInputCursorPos = 0;

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
    print("   #####  #####  #####");
    print("   ##     ##        ##");
    print("   #####  #####   ##  ");
    print("   ##        ##  ##   ");
    print("   #####  #####  #####");
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

    if (lineInputLength >= TERMINAL_BUFFER_SIZE) {
        displayTerminalError(String("Command exceeds ", TERMINAL_BUFFER_SIZE, " chars"));
        return;
    }

    add_command_to_history(lineInputBuffer);

    const char* args = getInputArgs(lineInputLength, lineInputBuffer);
    if (executeCommand(lineInputBuffer, args)) {
        lineInputLength = 0;
        lineInputBuffer[0] = 0;
    } else {
        displayTerminalError(String("Unknown Command: '", lineInputBuffer, "'"));
    }
}

void newTerminalInputLine() {
    const char* linePrefix = "esz >> ";
    print_inline(linePrefix);
}

void replaceCurrentToken(const char* oldToken, const char* newToken) {
    const uint64_t oldTokenLength = str_length(oldToken);
    for (uint64_t i = 0; i < oldTokenLength; i++) {
        cursor_backspace();
    }

    lineInputLength -= oldTokenLength;
    lineInputLength += str_length(newToken);

    str_trim_end(lineInputBuffer, oldTokenLength);
    str_add(lineInputBuffer, newToken);

    print_chars(newToken, true);
}

void handleTabAutoCompletion() {
    if (lineInputLength == 0) return;

    uint64_t maxCommandCounter = 0;
    for (uint64_t i = 0; commands[i].name != 0; i++) {
        maxCommandCounter++;
    }

    const char* lastToken = getLastTokenFromBuffer(lineInputLength, lineInputBuffer);
    if (lastToken[0] == 0) return;

    uint64_t validTabCannidates = 0;
    Command cannidateList[maxCommandCounter];
    
    // match input to list
    for (uint64_t i = 0; commands[i].name != 0; i++) {
        if (str_starts_with(commands[i].name, lastToken)) {
            cannidateList[validTabCannidates] = commands[i];
            validTabCannidates++;
        }
    }

    if (validTabCannidates == 0) return;

    // == 1 -> autocomplete directly
    if (validTabCannidates == 1) {
        replaceCurrentToken(lastToken, cannidateList[0].name);
        return;
    };

    // > 1 -> show possibilities in newline and refill the input in another newline
    // more tabs -> rotate through
    if (validTabCannidates > 1) {
        newline();
        for(uint64_t i = 0; i < validTabCannidates; i++) {
            print_chars(cannidateList[i].name, false);
            if (i != validTabCannidates - 1) print_chars(" | ", false);
        }

        newline();
        newTerminalInputLine();
        print_chars(lineInputBuffer, true);

        //TODO rotation

        return;
    }
}

void cursor_move_inline(const bool move_right) {
    if (move_right) {
        if (lineInputCursorPos >= lineInputLength) return;
        cursorAt_X++;
        lineInputCursorPos++;
        update_cursor_render();
    } else {
        if (lineInputCursorPos == 0) return;
        cursorAt_X--;
        lineInputCursorPos--;
        update_cursor_render();
    }
}

void handle_input_buffer_deletion() {
    if (lineInputLength == 0) return;
    if (lineInputCursorPos == 0) return;

    for (uint64_t i = lineInputCursorPos - 1; i < lineInputLength - 1; i++) {
        lineInputBuffer[i] = lineInputBuffer[i + 1];
    }
    lineInputLength--;
    lineInputBuffer[lineInputLength] = 0;

    Line* line = get_screen_buffer_line(cursorAt_Y);
    const unsigned int amountOfCells = (int)line->amountOfCells;
    for (int i = cursorAt_X - 1; i < amountOfCells - 1; i++) {
        line->cells[i] = line->cells[i + 1];
    }
    line->cells[amountOfCells - 1].text = 0;
    line->cells[amountOfCells - 1].interactable = false;
    line->amountOfCells--;

    lineInputCursorPos--;
    cursorAt_X--;

    const int savedX = cursorAt_X;

    isRedrawing = true;
    redraw_line(cursorAt_Y);
    isRedrawing = false;

    cursorAt_X = savedX;
    update_cursor_render();
}

void handle_input_buffer_insertion(const unsigned char scancode) {
    const char c = scancode_to_keycode(scancode);
    if (!c) return;

    for (uint64_t i = lineInputLength; i > lineInputCursorPos; i--) {
        lineInputBuffer[i] = lineInputBuffer[i - 1];
    }
    lineInputBuffer[lineInputCursorPos] = c;
    lineInputLength++;
    lineInputBuffer[lineInputLength] = 0;

    Line* line = get_screen_buffer_line(cursorAt_Y);
    for (int i = (int)line->amountOfCells; i > (int)cursorAt_X; i--) {
        line->cells[i] = line->cells[i - 1];
    }
    line->cells[cursorAt_X].text = c;
    line->cells[cursorAt_X].interactable = true;
    line->cells[cursorAt_X].color = Color::White;
    line->amountOfCells++;

    lineInputCursorPos++;
    cursorAt_X++;

    const int savedX = cursorAt_X;

    isRedrawing = true;
    redraw_line(cursorAt_Y);
    isRedrawing = false;
    
    cursorAt_X = savedX;
    update_cursor_render();
}

void terminal_on_key(const unsigned char scancode) {
    uint16_t key = scancode_to_keycode(scancode);
    
    if (isExtendedScancode) {
        switch(scancode) {
            case 0x48: // ARROW UP
                if (cmdHistCount == 0) break;
                
                if (goThroughHistoryCount < cmdHistCount) {
                    goThroughHistoryCount++;   
                } else break;
                
                handle_show_history();
                break;

            case 0x50: // ARROW DOWN
                if (goThroughHistoryCount == 0) break;
                goThroughHistoryCount--;

                if (goThroughHistoryCount == 0) {
                    delete_unprotected_chars();
                    lineInputBuffer[0] = 0;
                    lineInputLength = 0;
                } else {
                    handle_show_history();
                }
                break;

            case 0x4B: // ARROW LEFT
                cursor_move_inline(false);
                break;

            case 0x4D: // ARROW RIGHT
                cursor_move_inline(true);
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
            handle_input_buffer_deletion();
            break;

        case KeyCode::KEY_TAB:
            handleTabAutoCompletion();
            break;

        default:
            handle_input_buffer_insertion(scancode);
            break;
    }
}

void terminal_init() {
    printInfoLine(InfoTextType::Loading, "Initializing Terminal...");
        
    printHeader();
    newTerminalInputLine();
}
