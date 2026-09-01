#pragma once

enum class InfoTextType {
    Info,
    Warning,
    Error,
    Success,
    Loading,
    KernelPanic,
    PanicInfo,
    Debug
};

void printInfoLine(enum InfoTextType textType, const char* infoText);
