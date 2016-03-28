/*
 * File name: cpu.h
 * Description: x86 CPU HAL interface
 *
 * * * */

#ifndef CPU_H_
#define CPU_H_

#include <hal/gdt.h>

gdt_descriptor_t init_global_descriptor_table(void);

#endif /* CPU_H_ */
