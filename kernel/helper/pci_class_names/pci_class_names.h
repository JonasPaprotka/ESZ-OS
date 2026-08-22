#pragma once

#include <stdint.h>

struct PCIClassInfo {
    const char* className;
    const char* subClassName;
};

PCIClassInfo resolve_class_name(const uint8_t ClassCode, const uint8_t SubClass);
