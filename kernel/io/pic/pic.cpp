#include "io.h"
#include <stdint.h>

void EIO_Finished_Interrupt() {
    outb(0x20, 0x20);
}

void pic_init() {
    const uint8_t MASTER_CMD = 0x20; // 32
    const uint8_t MASTER_DATA = 0x21; // 33

    const uint8_t SLAVE_CMD = 0xA0; // 160
    const uint8_t SLAVE_DATA = 0xA1; // 161

    const uint8_t PIC_8086_MODE = 0x01;

    const uint8_t MASTER_IRQ_OFFSET = 0x20; // 32 first free vector after cpu exceptions
    const uint8_t SLAVE_IRQ_OFFSET  = 0x28; // 40

    // ICW1
    outb(MASTER_CMD, 0x11); // Init Master
    outb(SLAVE_CMD, 0x11); // Init Slave

    // ICW2
    outb(MASTER_DATA, MASTER_IRQ_OFFSET); // Master -> 0x20 (32)
    outb(SLAVE_DATA, SLAVE_IRQ_OFFSET); // Slave -> 0x28 (40)

    // ICW3
    outb(MASTER_DATA, 0x04); // Master -> 0x04
    outb(SLAVE_DATA, 0x02); // Slave -> 0x02

    // ICW4
    outb(MASTER_DATA, PIC_8086_MODE);
    outb(SLAVE_DATA, PIC_8086_MODE);

    // IRQs
    outb(MASTER_DATA, 0xFC); // turn IRQ0 (timer) and IRQ1 (keyboard) on
    outb(SLAVE_DATA, 0xFF); // turn all off
}
