#include "config.h"
#include "print.h"
#include "info_text.h"

void print_separator() {
    char* s = str_repeat("-", MAX_CHARS / 2);
    print_inline(s, Color::DarkGray);
    free(s);
    newline();
}

void printSysinfo() {
    printInfoLine(InfoTextType::Info, String(OS_NAME, " - ", ARCH_NAME, " - (", OS_VERSION_STRING, ") by ", OS_AUTHOR));
}

void printTerminalHeader() {
    print_separator();
    print("   #####  #####  #####");
    print("   ##     ##        ##");
    print("   #####  #####   ##  ");
    print("   ##        ##  ##   ");
    print("   #####  #####  #####");
    newline();
    printSysinfo();
    print_separator();
}


void printLoadingStart(const char* text) {
    print_inline("[LOADING]: ", Color::LightBlue);
    print_inline(text);
    print_inline("...");
}

void printLoadingStatus(const bool success) {
    if (success) {
        print_inline(" [OK]", Color::Green);
    } else {
        print_inline(" [NOK]", Color::Red);
    }
    newline();
}
