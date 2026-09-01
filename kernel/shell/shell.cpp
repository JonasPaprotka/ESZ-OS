#include "shell.h"
#include "line_editor.h"
#include "print.h"
#include "info_text.h"
#include "keyboard.h"
#include "keymap.h"
#include "parser.h"
#include "filesystem.h"
#include "string.h"
#include "history.h"
#include "autocomplete.h"
#include "print_helper.h"

void display_line_editor_error(const char* Text) {
    newline();
    printInfoLine(InfoTextType::Error, Text);
    reset_line_input();
}

void new_line_editor_input_line() {
    const char* renderPath = currentPath;
    String linePrefix(renderPath, " >> ");
    print_inline(linePrefix);
    lineInputStart_X = cursorAt_X;
}

static void process_line_input_buffer() {
    if (lineInputLength == 0) {
        newline();
        reset_line_input();
        return;
    }

    if (lineInputLength >= TERMINAL_BUFFER_SIZE) {
        display_line_editor_error(String("Command exceeds ", TERMINAL_BUFFER_SIZE, " chars"));
        return;
    }

    add_command_to_history(lineInputBuffer);

    const char* args = getInputArgs(lineInputLength, lineInputBuffer);
    switch (executeCommand(lineInputBuffer, args)) {
        case Return::Success:
            reset_line_input();
            break;
        case Return::Error:
            display_line_editor_error("An error occured");
            break;
        case Return::Warning:
            display_line_editor_error(String("Unknown Command: '", lineInputBuffer, "'"));
            break;
        case Return::NoReturn:
            display_line_editor_error("Well this is akward... this command should not be able to return.");
            break;
    }
}

void shell_on_key(const uint8_t scancode) {
    const uint16_t key = scancode_to_keycode(scancode, isExtendedScancode);
    isExtendedScancode = false;

    switch(key) {
        case KeyCode::KEY_ARROW_UP:
            if (cmdHistCount == 0) break;

            if (goThroughHistoryCount < cmdHistCount) {
                goThroughHistoryCount++;
            } else
                break;

            handle_show_history();
            break;

        case KeyCode::KEY_ARROW_DOWN:
            if (goThroughHistoryCount == 0) break;
            goThroughHistoryCount--;

            if (goThroughHistoryCount == 0) {
                clear_input_on_screen();
                reset_line_input();
            } else {
                handle_show_history();
            }
            break;

        case KeyCode::KEY_ARROW_LEFT:
            cursor_move_inline(false);
            break;

        case KeyCode::KEY_ARROW_RIGHT:
            cursor_move_inline(true);
            break;

        case KeyCode::KEY_ENTER:
            goThroughHistoryCount = 0;
            process_line_input_buffer();
            new_line_editor_input_line();
            break;

        case KeyCode::KEY_BACKSPACE:
            handle_input_buffer_deletion();
            break;

        case KeyCode::KEY_TAB:
            handleTabAutoCompletion();
            break;

        default:
            if (key != KeyCode::KEY_UNKNOWN && key < 256) insert_char_at_cursor(key);
            break;
    }
}

bool shell_init() {
    printTerminalHeader();
    new_line_editor_input_line();

    return true;
}