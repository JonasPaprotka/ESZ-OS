#ifndef INFO_HELPER_H
#define INFO_HELPER_H

#include "string.h"

enum class InfoTextType {
    Info,
    Warning,
    Error,
    Success,
    Loading,
    KernelPanic,
    PanicInfo
};

void printInfoLine(enum InfoTextType textType, const char* infoText);
void print_separator();

#endif
