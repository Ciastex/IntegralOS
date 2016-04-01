/*
 * File name: panic.h
 * Description: Kernel panic interface header
 *
 * * * */

#ifndef PANIC_H_
#define PANIC_H_

#include <hal/isr.h>

void kpanic(const char *message, uint32_t ex_data_0, uint32_t ex_data_1, uint32_t ex_data_2);

#endif /* PANIC_H_ */
