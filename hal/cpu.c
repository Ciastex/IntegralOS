/*
 * File name: cpu.c
 * Description: x86 CPU support implementation
 *
 * * * */
#include <stdbool.h>
#include <string.h>

#include <hal/cpu.h>
#include <hal/gdt.h>
#include <hal/idt.h>
#include <hal/isr.h>
#include <hal/irq.h>
