#ifndef IDT_H
#define IDT_H

#include <stdint.h>

void idt_init();
void idt_set_entry(int n, uint64_t handler);

#endif // IDT_H
