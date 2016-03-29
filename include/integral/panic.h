/*
 * File name: panic.h
 * Description: Kernel panic interface header
 *
 * * * */

#ifndef PANIC_H_
#define PANIC_H_

#include <hal/isr.h>

void kpanic(const char *message/*, isr_info_t *context*/);

#endif /* PANIC_H_ */
