#pragma once

#include "config.h"
#include "types.h"

struct Argument {
    const char* name;
    const char* summary = "";
    Types type = Types::Boolean;
    bool isRequired = false;
    char value[LINE_EDITOR_BUFFER_SIZE] = {};
};
