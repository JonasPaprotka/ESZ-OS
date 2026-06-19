#ifndef PCI_CLASS_NAMES_H
#define PCI_CLASS_NAMES_H

#include <stdint.h>
#include "string.h"

struct PCIClassInfo {
    const char* className;
    const char* subClassName;
};

PCIClassInfo resolve_class_name(const uint8_t ClassCode, const uint8_t SubClass);

#endif
