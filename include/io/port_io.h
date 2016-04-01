/*
 * File name: port_io.h
 * Description: 
 *
 * * * */

#ifndef PORT_IO_H_
#define PORT_IO_H_

#include <stdint.h>

#define SYSTEM_CONTROL_PORT_A 0x92
#define SYSTEM_CONTROL_PORT_B 0x61

void io_wait(void);

void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t value);
void outl(uint16_t port, uint32_t value);

uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);

#endif /* PORT_IO_H_ */
