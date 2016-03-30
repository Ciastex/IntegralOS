/*
 * File name: irq.c
 * Description: IRQ handler and monitor
 *
 * * * */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <integral/panic.h>

#include <hal/irq.h>
#include <io/8259a/pic.h>

static uint32_t spurious_irq_count = 0;
static irq_func_ptr handlers[256] = { 0 };

void irq_handler(irq_info_t *irq_info) {
    if(irq_info->irq_number == IRQ_NUMBER_LPT1) {               // Spurious IRQ for MASTER
        uint16_t combined_isr_state = pic_get_combined_isr();
        uint8_t master_isr_state = combined_isr_state & 0xFF;

        if(!IS_BIT_SET(master_isr_state, IRQ_NUMBER_LPT1)) {
            spurious_irq_count++;
            return;
        }
    } else if(irq_info->irq_number == IRQ_NUMBER_RSD5) {        // Spurious IRQ for SLAVE
        uint16_t combined_isr_state = pic_get_combined_isr();
        uint8_t slave_isr_state = (combined_isr_state >> 8) & 0xFF;

        if(!IS_BIT_SET(slave_isr_state, IRQ_NUMBER_RSD5)) {
            spurious_irq_count++;
            pic_send_eoi_master();
            return;
        }
    }

    if(handlers[irq_info->irq_number]) {
        (*(handlers[irq_info->irq_number]))(irq_info);
    }

    pic_send_eoi(irq_info->irq_number);
}

void set_irq_handler(uint32_t number, uint32_t func_pointer) {
    if(number > 256) {
        kpanic("Tried to set IRQ handler with number greater than 256");
    }
    handlers[number] = (irq_func_ptr)func_pointer;
}

uint32_t get_spurious_irq_count(void) {
    return spurious_irq_count;
}
