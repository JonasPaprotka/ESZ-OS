#ifndef COMMANDS_H
#define COMMANDS_H
#include "string.h"

struct Command {
    cstr name;
    void (*execute)(const char* args);
};

extern Command commands[];

#endif
