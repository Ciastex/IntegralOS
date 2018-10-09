/*
 * kernel.h
 * Kernel header file. Contains the kernel interface.
 *
 * * * */

#ifndef KERNEL_H_
#define KERNEL_H_

#if defined(__linux__)
#error "This operating system kernel requires i686-elf target cross-compiler."
#endif

#if !defined(__i386__)
#error "This operating system kernel requires an i86 host machine."
#endif

#define INTEGRAL_VERSION "0.02 built at " __TIME__ " on "__DATE__

#define TTY_KERNEL 7
#define TTY_FAULTS 6

void printk(uint8_t terminal_index, char* str, ...);

#endif /* KERNEL_H_ */
