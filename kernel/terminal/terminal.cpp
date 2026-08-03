#include "terminal.h"
#include "color.h"
#include "print.h"
#include "print_helper.h"
#include "screenBuffer.h"
#include "info_text.h"
#include "keyboard.h"
#include "keymap.h"
#include "args.h"
#include "commands.h"
#include "filesystem.h"
#include "heap.h"
#include "string.h"

uint64_t lineInputLength = 0;
char lineInputBuffer[TERMINAL_BUFFER_SIZE];
unsigned int lineInputCursorPos;
unsigned int lineInputStart_X;

char commandHistory[MAX_COMMAND_HISTORY][TERMINAL_BUFFER_SIZE];
uint64_t cmdHistCount = 0;
uint64_t goThroughHistoryCount = 0;

void reset_line_input() {
    lineInputLength = 0;
    lineInputCursorPos = 0;
    lineInputBuffer[0] = 0;
}

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

void clear_input_on_screen() {
    Line* line = get_screen_buffer_line(cursorAt_Y);

    const int totalCells = (int) line->amountOfCells;
    for (int i = lineInputStart_X; i < totalCells && i < MAX_CHARS; i++) {
        Cell* cell = &line->cells[i];
        cell->text = 0;
        cell->interactable = false;
        cell->color = Color::White;
    }
    line->amountOfCells = lineInputStart_X;

    isRedrawing = true;
    redraw_line(cursorAt_Y);
    isRedrawing = false;

    cursorAt_X = lineInputStart_X;
    lineInputCursorPos = 0;
    update_cursor_render();
}

void handle_show_history() {
    clear_input_on_screen();
    str_copy(lineInputBuffer, commandHistory[cmdHistCount - goThroughHistoryCount]);
    lineInputLength = str_length(lineInputBuffer);
    lineInputCursorPos = lineInputLength;
    print_chars(lineInputBuffer, true);
}

void displayTerminalError(const char* Text) {
    newline();
    printInfoLine(InfoTextType::Error, Text);
    reset_line_input();
}

void processLineInputBuffer() {
    if (lineInputLength == 0) {
        newline();
        reset_line_input();
        return;
    }

    if (lineInputLength >= TERMINAL_BUFFER_SIZE) {
        displayTerminalError(String("Command exceeds ", TERMINAL_BUFFER_SIZE, " chars"));
        return;
    }

    add_command_to_history(lineInputBuffer);

    const char* args = getInputArgs(lineInputLength, lineInputBuffer);
    if (executeCommand(lineInputBuffer, args)) {
        reset_line_input();
    } else {
        displayTerminalError(String("Unknown Command: '", lineInputBuffer, "'"));
    }
}

void newTerminalInputLine() {
    const char* renderPath = currentPath;
    const char* linePrefix = String(renderPath, " >> ");
    print_inline(linePrefix);
    lineInputStart_X = cursorAt_X;
}

void replaceCurrentToken(const char* oldToken, const char* newToken) {
    while (lineInputCursorPos < lineInputLength && lineInputBuffer[lineInputCursorPos] != ' ')
        cursor_move_inline(true);

    for (uint64_t i = 0; i < str_length(oldToken); i++)
        handle_input_buffer_deletion();

    for (uint64_t i = 0; i < str_length(newToken); i++)
        insert_char_at_cursor(newToken[i]);
}

void handleTabAutoCompletion() {
    if (lineInputLength == 0) return;

    uint64_t maxCommandCounter = 0;
    for (uint64_t i = 0; commands[i].name != 0; i++) {
        maxCommandCounter++;
    }

    const char* currentToken = getCurrTokenFromBuffer(lineInputCursorPos, lineInputLength, lineInputBuffer);
    if (currentToken[0] == 0) {
        free(currentToken);
        return;
    }

    uint64_t validTabCannidates = 0;
    Command cannidateList[maxCommandCounter];

    // match input to list
    for (uint64_t i = 0; commands[i].name != 0; i++) {
        if (str_starts_with(commands[i].name, currentToken)) {
            cannidateList[validTabCannidates] = commands[i];
            validTabCannidates++;
        }
    }

    if (validTabCannidates == 0) {
        free(currentToken);
        return;
    }

    // == 1 -> autocomplete directly
    if (validTabCannidates == 1) {
        replaceCurrentToken(currentToken, cannidateList[0].name);
        free(currentToken);
        return;
    };

    free(currentToken);

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
        lineInputCursorPos = lineInputLength;

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

    Line* line = get_screen_buffer_line(cursorAt_Y);
    if (cursorAt_X <= 0) return;
    if (!line->cells[cursorAt_X - 1].interactable) return;

    for (uint64_t i = lineInputCursorPos - 1; i < lineInputLength - 1; i++) {
        lineInputBuffer[i] = lineInputBuffer[i + 1];
    }
    lineInputLength--;
    lineInputBuffer[lineInputLength] = 0;

    const int amountOfCells = (int) line->amountOfCells;
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

void insert_char_at_cursor(const char c) {
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

void handle_input_buffer_insertion(const uint8_t scancode) {
    const char c = scancode_to_keycode(scancode);
    if (!c) return;
    insert_char_at_cursor(c);
}

void terminal_on_key(const uint8_t scancode) {
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
                    clear_input_on_screen();
                    reset_line_input();
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
    printTerminalHeader();
    newTerminalInputLine();
}
