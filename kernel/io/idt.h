#ifndef IDT_H
#define IDT_H

void idt_init();
void idt_set_entry(int n, unsigned int handler);

#endif
