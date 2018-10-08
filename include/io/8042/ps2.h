#ifndef INCLUDE_IO_8042_PS2_H_
#define INCLUDE_IO_8042_PS2_H_

#include <stdint.h>
#include <stdbool.h>

#define PS2_PORT_IO 0x60
#define PS2_REG_CMD 0x64
#define PS2_REG_STA 0x64

#define PS2_PORT1 0
#define PS2_PORT2 1

#define PS2_STATUS_OUTPUT   0x01
#define PS2_STATUS_INPUT    0x02
#define PS2_STATUS_SYSTEM   0x04
#define PS2_STATUS_CMD_DATA 0x08
#define PS2_STATUS_LOCKED   0x10
#define PS2_STATUS_AUX_BUF  0x20
#define PS2_STATUS_TIMEOUT  0x40
#define PS2_STATUS_PARITY   0x80

#define PS2_CMD_GETCONFIGBYTE 0x20
#define PS2_CMD_SETCONFIGBYTE 0x60
#define PS2_CMD_TESTCNTRLR    0xAA
#define PS2_CMD_DISABLEPORT_1 0xAD
#define PS2_CMD_ENABLEPORT_1  0xAE
#define PS2_CMD_TESTPORT_1    0xAB
#define PS2_CMD_DISABLEPORT_2 0xA7
#define PS2_CMD_ENABLEPORT_2  0xA8
#define PS2_CMD_TESTPORT_2    0xA9
#define PS2_CMD_READ_OUTPUT   0xD0
#define PS2_CMD_WRITE_OUTPUT  0xD1
#define PS2_CMD_SWITCH_PORT2  0xD4
#define PS2_CMD_PULSE_RESETLN 0xFE

#define PS2_DEVCMD_ENABLE_SCANNING 0xF4
#define PS2_DEVCMD_DISABLE_SCANNING 0xF5
#define PS2_DEVCMD_IDENTIFY         0xF2
#define PS2_DEVRESPONSE_ACK         0xFA

#define PS2_TESTRESULT_PORT_OK    0x00
#define PS2_TESTRESULT_CLOCK_LOW  0x01
#define PS2_TESTRESULT_CLOCK_HIGH 0x02
#define PS2_TESTRESULT_DATA_LOW   0x03
#define PS2_TESTRESULT_DATA_HIGH  0x04
// ---
#define PS2_TESTRESULT_CNTRLR_OK  0x55
#define PS2_TESTRESULT_CNTRLR_ERR 0xFC

#define PS2_MAX_RETRIES 32

typedef uint8_t ps2_status_t;
typedef uint8_t ps2_word_t;
typedef uint8_t ps2_flags_t;
typedef uint8_t ps2_port_t;

typedef enum ps2_device_type {
    MOUSE_STANDARD = 0x00,
    MOUSE_SCROLLWHEEL = 0x01,
    MOUSE_FIVEBUTTON = 0x02,
    KEYBOARD_TRANSLATING = 0x03,
    KEYBOARD = 0x04
} ps2_devtype_t;

typedef struct ps2_port_info {
    bool operational;
    ps2_devtype_t device;
} ps2_port_info_t;

typedef struct ps2_driver_state {
    ps2_port_info_t ports[2];
    bool is_dual_channel;
    ps2_word_t last_packet_sent;
    ps2_port_t last_port_used;
} ps2_drv_state_t;

typedef struct ps2_data {
    bool timeout;
    ps2_word_t response;
} ps2_data_t;

void ps2_initialize(void);
void ps2_send(ps2_port_t port, ps2_word_t packet);
void ps2_resend_last(void);
bool ps2_read(ps2_word_t* retval);
ps2_status_t ps2_read_status(void);
ps2_word_t ps2_get_configuration_byte(void);
void ps2_set_configuration_byte(ps2_word_t new_value);
void ps2_set_port_state(uint8_t port, bool enable);
void ps2_flush_data_port(void);
void ps2_wait_for_data(ps2_data_t* out_status);
ps2_devtype_t ps2_identify_device(uint8_t port);
bool ps2_test_controller(ps2_word_t* out_response);
bool ps2_test_port(uint8_t port, ps2_word_t* out_response);
void ps2_reset_cpu(void);
ps2_drv_state_t ps2_get_drv_state(void);

#endif // INCLUDE_IO_8042_PS2_H_
