#include "vga.h"
#include "info_text.h"
#include "string.h"

void printInfoLine(const enum InfoTextType textType, cstr infoText) {
    Color textTagColor = White;
    cstr textTagText = "";

    switch (textType) {
        case Info:
            textTagColor = LightCyan;
            textTagText = "[INFO]: ";
            break;
        case Warning:
            textTagColor = Yellow;
            textTagText = "[WARNING]: ";
            break;
        case Error:
            textTagColor = Red;
            textTagText = "[ERROR]: ";
            break;
        case Success:
            textTagColor = Green;
            textTagText = "[SUCCESS]: ";
            break;
        case Loading:
            textTagColor = LightBlue;
            textTagText = "[LOADING]: ";
            break;
    }

    print_inline(textTagText, textTagColor);
    print_inline(infoText);
    newline();
}

void printSeperator() {
    print_inline(str_repeat("-", 40), DarkGray);
    newline();
}
