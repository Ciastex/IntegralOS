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

#endif /* KERNEL_H_ */
