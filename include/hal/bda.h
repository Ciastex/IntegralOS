#ifndef BDA_H_
#define BDA_H_

#include <stdint.h>

typedef struct bda_info {
	uint16_t com_port1_addr;
	uint16_t com_port2_addr;
	uint16_t com_port3_addr;
	uint16_t com_port4_addr;

	uint16_t lpt_port1_addr;
	uint16_t lpt_port2_addr;
	uint16_t lpt_port3_addr;

	uint16_t ebda_addr;
	uint16_t hw_flags;
} bda_info_t;

bda_info_t bda_get_info();

#endif // BDA_H_
