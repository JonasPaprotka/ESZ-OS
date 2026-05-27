#include "info_text.h"
#include "string.h"
#include "color.h"
#include "print.h"

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
    }

    print_inline(textTagText, textTagColor);
    print_inline(infoText);
    newline();
}

void printSeperator() {
    //print_inline(str_repeat("-", 40), Color::DarkGray);
    print_inline("----------------------------------------", Color::DarkGray);
    newline();
}
