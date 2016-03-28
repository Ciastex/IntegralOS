/*
 * File name: cpu.h
 * Description: x86 CPU HAL interface
 *
 * * * */

#ifndef CPU_H_
#define CPU_H_

#include <hal/gdt.h>
#include <hal/idt.h>

#define NULL_SELECTOR              0x00
#define KERNELMODE_CODE_SELECTOR   0x08
#define KERNELMODE_DATA_SELECTOR   0x10
#define USERMODE_CODE_SELECTOR     0x18
#define USERMODE_DATA_SELECTOR     0x20

gdt_descriptor_t init_global_descriptor_table(void);
idt_descriptor_t init_interrupt_descriptor_table(void);

#endif /* CPU_H_ */
