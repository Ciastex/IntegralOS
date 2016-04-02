/*
 * File name: pit.h
 * Description: Programmable Interval Timer driver interface
 *  
 * * * * */

#ifndef INCLUDE_IO_8259A_PIT_H_
#define INCLUDE_IO_8259A_PIT_H_

#define PIT_CHANNEL0_PORT 0x40
#define PIT_CHANNEL1_PORT 0x41
#define PIT_CHANNEL2_PORT 0x42
#define PIT_COMMAND_PORT  0x43 /* NOTE: This is write-only, read is ignored. */

typedef void (* pit_callback_ptr)(void);

void pit_init(void);
void pit_set_callback(uintptr_t address);
void pit_unset_callback(void);
uint32_t pit_get_total_ticks(void);

#endif /* INCLUDE_IO_8259A_PIT_H_ */
