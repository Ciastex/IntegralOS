/*
 * File name: pit.c
 * Description: Programmable Interval Timer driver implementation
 *  
 * * * * */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <hal/irq.h>
#include <io/port_io.h>
#include <io/8259a/pit.h>

static void pit_internal_callback(irq_info_t *irq_info);

static uint32_t total_ticks = 0;
static pit_callback_ptr pit_callback = 0;

void pit_init(void) {
	irq_set_handler(IRQ_NUMBER_PIT, (uintptr_t)&pit_internal_callback);
}

void pit_set_callback(uintptr_t address) {
	pit_callback = (pit_callback_ptr)address;
}

void pit_unset_callback(void) {
	pit_callback = 0;
}

uint32_t pit_get_total_ticks(void) {
	return total_ticks;
}

static void pit_internal_callback(irq_info_t *irq_info) {
	total_ticks++;

	if(pit_callback) {
		(*(pit_callback))();
	}
}
