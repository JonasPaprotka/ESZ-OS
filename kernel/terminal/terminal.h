#ifndef TERMINAL_H
#define TERMINAL_H

void terminal_init();
void terminal_on_key(unsigned char scancode);
extern int cursorAtChar;
extern int cursorAtLine;

#endif
