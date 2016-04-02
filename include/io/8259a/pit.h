/*
 * File name: pit.h
 * Description: Programmable Interval Timer driver interface
 *  
 * * * * */

#ifndef INCLUDE_IO_8259A_PIT_H_
#define INCLUDE_IO_8259A_PIT_H_

#include <stdint.h>

#define PIT_CHANNEL0_PORT 0x40
#define PIT_CHANNEL1_PORT 0x41
#define PIT_CHANNEL2_PORT 0x42
#define PIT_COMMAND_PORT  0x43 /* NOTE: This is write-only, read is ignored. */

#define PIT_CMD_BCD                          0x0001
#define PIT_CMD_MODE_HARDWARE_RETRIG_ONESHOT 0x0002
#define PIT_CMD_MODE_RATE_GENERATOR          0x0004
#define PIT_CMD_MODE_SQUARE_WAVE             0x0006
#define PIT_CMD_MODE_SOFTWARE_STROBE         0x0008
#define PIT_CMD_MODE_HARDWARE_STROBE         0x000A
#define PIT_CMD_RW_MODE_READ_LSB             0x0010
#define PIT_CMD_RW_MODE_READ_MSB             0x0020
#define PIT_CMD_RW_MODE_READ_LSB_MSB         0x0030 /* LSB, then MSB */
#define PIT_CMD_COUNTER_1                    0x0040
#define PIT_CMD_COUNTER_2                    0x0080

typedef void (* pit_callback_ptr)(void);

void pit_init(void);
void pit_set_callback(uintptr_t address);
void pit_unset_callback(void);
void pit_set_frequency(uint16_t frequency);
uint16_t pit_get_current_frequency(void);
uint32_t pit_get_total_ticks(void);

#endif /* INCLUDE_IO_8259A_PIT_H_ */
