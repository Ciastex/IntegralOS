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
static irq_func_ptr handlers[IRQ_MAX_HANDLERS] = { 0 };

void irq_handler(irq_info_t *irq_info) {
    uint16_t combined_isr_state = pic_get_combined_isr();

    if(irq_info->irq_number >= 8) {
        uint8_t slave_isr_state = (combined_isr_state >> 8) & 0xFF;

        if(!IS_BIT_SET(slave_isr_state, irq_info->irq_number)) {
            spurious_irq_count++;
            pic_send_eoi_master();
            return;
        }
    } else {
        uint8_t master_isr_state = combined_isr_state & 0xFF;

        if(!IS_BIT_SET(master_isr_state, irq_info->irq_number)) {
            spurious_irq_count++;
            return;
        }
    }

    if(handlers[irq_info->irq_number]) {
        (*(handlers[irq_info->irq_number]))(irq_info);
    }

    pic_send_eoi(irq_info->irq_number);
}

void irq_set_handler(uint32_t number, uintptr_t func_pointer) {
    if(number > IRQ_MAX_HANDLERS - 1) {
        kpanic("Tried to define IRQ ISR index greater than 255.", number, func_pointer, 0);
    }
    handlers[number] = (irq_func_ptr)func_pointer;
}

uint32_t irq_get_spurious_count(void) {
    return spurious_irq_count;
}
