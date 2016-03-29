/*
 * File name: pic.c
 * Description: The 8259A Programmable Interrupt Controller
 *
 * * * */
#include <stdint.h>
#include <io/8259a/pic.h>
#include <io/port_io.h>

void pic_remap(uint32_t master_start_vector, uint32_t slave_start_vector) {
    uint8_t master_mask = inb(PIC8259_MASTER_DATA);
    uint8_t slave_mask = inb(PIC8259_SLAVE_DATA);

    /* Master and slave initialization */
    outb(PIC8259_MASTER_CMD, PIC8259_CMD_INIT);
    io_wait();
    outb(PIC8259_SLAVE_CMD, PIC8259_CMD_INIT);
    io_wait();

    /* Actual PIC IRQ remap occurs here */
    outb(PIC8259_MASTER_DATA, master_start_vector);
    io_wait();
    outb(PIC8259_SLAVE_DATA, slave_start_vector);
    io_wait();

    /* Master PIC environment set-up */
    outb(PIC8259_MASTER_DATA, PIC_THERE_IS_A_SLAVE_AT_2);
    io_wait();
    outb(PIC8259_SLAVE_DATA, PIC_CASCADE_ID_2);
    io_wait();

    /* Set operation mode */
    outb(PIC8259_MASTER_DATA, PIC_MODE_8086);
    io_wait();
    outb(PIC8259_SLAVE_DATA, PIC_MODE_8086);
    io_wait();

    /* Restoring all previous masks */
    outb(PIC8259_MASTER_DATA, master_mask);
    outb(PIC8259_SLAVE_DATA, slave_mask);
}

void pic_send_eoi(uint8_t irq_number) {
    if(irq_number >= 8) {
        outb(PIC8259_SLAVE_CMD, PIC8259_CMD_EOI);
    }
    outb(PIC8259_MASTER_CMD, PIC8259_CMD_EOI);
}
