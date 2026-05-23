#ifndef COMMANDS_H
#define COMMANDS_H

#include "string.h"

struct Command {
    string name;
    void (*execute)(const char* args);
};

extern Command commands[];

#endif
