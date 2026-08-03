#ifndef COMMANDS_H
#define COMMANDS_H

struct Command {
    const char* name;
    void (*execute)(const char* args);
};

void cmd_sysinfo(const char* = nullptr);

extern const Command commands[];

#endif // COMMANDS_H
