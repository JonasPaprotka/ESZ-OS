#ifndef COMMANDS_H
#define COMMANDS_H

#include "string.h"

struct Command {
    const char* name;
    void (*execute)(const char* args);
};

extern const Command commands[];

#endif
