/*
 * File name: nmi_eh.c
 * Description:
 *
 * * * * */
#include <integral/exception.h>
#include <integral/panic.h>
#include <io/port_io.h>
#include <hal/isr.h>

void non_maskable_interrupt_handler(isr_info_t *isr_info) {
    uint8_t control_port_a = inb(SYSTEM_CONTROL_PORT_A);
    uint8_t control_port_b = inb(SYSTEM_CONTROL_PORT_B);

    kpanic("Non-maskable interrupt occured.", isr_info->eip, (uint32_t)control_port_a, (uint32_t)control_port_b);
}
