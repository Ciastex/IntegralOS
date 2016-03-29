/*
 * File name: pic.h
 * Description: 
 *
 * * * */

#ifndef PIC_H_
#define PIC_H_

#include <stdint.h>

#define PIC8259_MASTER_CMD  0x20
#define PIC8259_MASTER_DATA (PIC8259_MASTER_CMD + 1)
#define PIC8259_SLAVE_CMD   0xA0
#define PIC8259_SLAVE_DATA  (PIC8259_SLAVE_CMD + 1)

#define PIC8259_CMD_INIT     0x11
#define PIC8259_CMD_DISABLE  0xFF
#define PIC8259_CMD_EOI      0x20
#define PIC8259_CMD_READ_IRR 0x0A
#define PIC8259_CMD_READ_ISR 0x0B

#define PIC_SLAVE_AT_IRQ2 0x04
#define PIC_CASCADE_IRQ2 0x02

#define PIC_MODE_8086       0x01
#define PIC_MODE_AUTO       0x02
#define PIC_MODE_BUF_SLAVE  0x08
#define PIC_MODE_BUF_MASTER 0x0C
#define PIC_SPECIAL_NESTED  0x10

void pic_remap(uint32_t master_start_vector, uint32_t slave_start_vector);
void pic_disable(void);

uint16_t pic_get_reg_value(uint8_t ocw);
uint16_t pic_get_combined_irr(void);
uint16_t pic_get_combined_isr(void);

void pic_send_eoi_master(void);
void pic_send_eoi(uint8_t irq_number);

#endif /* PIC_H_ */
