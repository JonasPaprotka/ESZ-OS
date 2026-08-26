#pragma once

struct Command {
    const char* name;
    void (*execute)(const char* args);
};

extern const Command commands[];
