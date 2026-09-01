#include "print_helper.h"
#include "config.h"
#include "color.h"
#include "print.h"
#include "info_text.h"
#include "memory.h"
#include "heap.h"
#include "string.h"

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
    if (success) print_inline(" [OK]", Color::Green);
    else print_inline(" [NOK]", Color::Red);
    newline();
}

char* getPercentBar(const uint64_t count, const uint64_t outOf, const uint64_t elements, const bool showPercentage) {
    if (outOf == 0) return malloc_empty_str();

    const uint64_t size = elements + 9;
    char* outText = malloc_str(size);
    memory_clear(outText, size);

    outText[0] = '[';
    outText[elements + 1] = ']';

    if (showPercentage) outText[elements + 8] = 0;
    else outText[elements + 2] = 0;

    const uint64_t percentage = (count * 100) / outOf;
    const uint64_t elementCount = (percentage * elements) / 100;;

    for (uint64_t i = 1; i < elementCount + 1; i++) {
        outText[i] = '#';
    }

    if (elements > elementCount) {
        for (uint64_t i = elementCount + 1; i < elements + 1; i++) {
            outText[i] = '-';
        }
    }

    if (showPercentage) {
        str_add(outText, String(" ", percentage, "%"));
    }

    return outText;
}

void printPercentBar(const uint64_t count, const uint64_t outOf, const uint64_t elements, const bool showPercentage) {
    char* percentBarText = getPercentBar(count, outOf, elements, showPercentage);
    print(percentBarText);
    free(percentBarText);
}
