/*
 * File name: irq.c
 * Description: IRQ handlers
 *
 * * * */
#include <hal/irq.h>
#include <io/8259a/pic.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t timer_ticks = 0;

void irq_handler(irq_info_t *irq_info) {
    if(irq_info->irq_number == 0) {
        timer_ticks++;
        printf("\rIRQ_0: %d", timer_ticks);
    }

    pic_send_eoi(irq_info->irq_number);
}
