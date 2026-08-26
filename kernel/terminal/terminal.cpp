#include "terminal.h"
#include "print.h"
#include "print_helper.h"
#include "screenBuffer.h"
#include "info_text.h"
#include "keyboard.h"
#include "keymap.h"
#include "args.h"
#include "filesystem.h"
#include "string.h"
#include "cmd_history.h"
#include "autocomplete.h"

uint64_t lineInputLength = 0;
char lineInputBuffer[TERMINAL_BUFFER_SIZE];
unsigned int lineInputCursorPos;
unsigned int lineInputStart_X;

static void reset_line_input() {
    lineInputLength = 0;
    lineInputCursorPos = 0;
    lineInputBuffer[0] = 0;
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

static void displayTerminalError(const char* Text) {
    newline();
    printInfoLine(InfoTextType::Error, Text);
    reset_line_input();
}

static void processLineInputBuffer() {
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
    String linePrefix(renderPath, " >> ");
    print_inline(linePrefix);
    lineInputStart_X = cursorAt_X;
}

static void insert_char_at_cursor(const char c) {
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

static void handle_input_buffer_insertion(const uint8_t scancode) {
    const char c = scancode_to_keycode(scancode);
    if (!c) return;
    insert_char_at_cursor(c);
}

void replaceCurrentToken(const char* oldToken, const char* newToken) {
    while (lineInputCursorPos < lineInputLength && lineInputBuffer[lineInputCursorPos] != ' ')
        cursor_move_inline(true);

    for (uint64_t i = 0; i < str_length(oldToken); i++)
        handle_input_buffer_deletion();

    for (uint64_t i = 0; i < str_length(newToken); i++)
        insert_char_at_cursor(newToken[i]);
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
