#pragma once

struct Command {
    const char* name;
    void (*execute)(const char* args);
};

void cmd_sysinfo(const char* = nullptr);

extern const Command commands[];
