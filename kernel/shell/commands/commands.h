#pragma once

#include "args.h"
#include "return.h"

struct Command {
    const char* name;
    const char* summary = "";
    Return (*execute)(const Command& self);
    Argument args[MAX_COMMAND_ARGS] = {};
};

extern Command commands[];
