#ifndef INFO_HELPER_H
#define INFO_HELPER_H
#include "string.h"

enum InfoTextType {
    Info,
    Warning,
    Error,
    Success,
    Loading
};

void printInfoLine(enum InfoTextType textType, cstr infoText);
void printSeperator();

#endif
