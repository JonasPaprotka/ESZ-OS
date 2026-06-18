#include "info_text.h"
#include "string.h"
#include "color.h"
#include "print.h"
#include "memory.h"

void printInfoLine(const enum InfoTextType textType, const char* infoText) {
    Color textTagColor = Color::White;
    const char* textTagText = "";

    switch (textType) {
        case InfoTextType::Info:
            textTagColor = Color::LightCyan;
            textTagText = "[INFO]: ";
            break;
        case InfoTextType::Warning:
            textTagColor = Color::Yellow;
            textTagText = "[WARNING]: ";
            break;
        case InfoTextType::Error:
            textTagColor = Color::Red;
            textTagText = "[ERROR]: ";
            break;
        case InfoTextType::Success:
            textTagColor = Color::Green;
            textTagText = "[SUCCESS]: ";
            break;
        case InfoTextType::Loading:
            textTagColor = Color::LightBlue;
            textTagText = "[LOADING]: ";
            break;
        case InfoTextType::KernelPanic:
            textTagColor = Color::Red;
            textTagText = "[KERNEL PANIC]: ";
            break;
        case InfoTextType::PanicInfo:
            textTagColor = Color::LightRed;
            textTagText = "[PANIC INFO]: ";
            break;
        case InfoTextType::Debug:
            textTagColor = Color::HotPink;
            textTagText = "[DEBUG]: ";
    }

    print_inline(textTagText, textTagColor);
    print_inline(infoText);
    newline();
}

void print_separator() {
    char* s = str_repeat("-", MAX_CHARS / 3);
    print_inline(s, Color::DarkGray);
    free(s);
    newline();
}
