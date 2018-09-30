#include <hal/bda.h>

bda_info_t bda_get_info() {
	bda_info_t* data_pointer = 0x0400;
	return *data_pointer;
}
