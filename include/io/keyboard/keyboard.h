/*
 * File name: keyboard.h
 * Description: Keyboard driver interface
 *  
 * * * * */

#ifndef INCLUDE_IO_KEYBOARD_KEYBOARD_H_
#define INCLUDE_IO_KEYBOARD_KEYBOARD_H_

#include <stdbool.h>
#include <stdint.h>

#define KBD_ENCODER_INPUT_BUFFER 0x60
#define KBD_ENCODER_COMMAND_PORT 0x60
#define KBD_CONTROL_STATUS_REG   0x64
#define KBD_CONTROL_COMMAND_REG  0x64

#define KBDMSG_ERROR     0x00 /* Error message */
#define KBDMSG_BAT_OK    0xAA /* Basic Assurance Test OK */
#define KBDMSG_ECHO_RES  0xEE /* Echo command result */
#define KBDMSG_PASSWORD  0xF1 /* No password set (response for command 0xA4) */
#define KBDMSG_ACK       0xFA /* Acknowledged message */
#define KBDMSG_BATERR    0xFC /* Basic Assurance Test error */
#define KBDMSG_INTER_ERR 0xFD /* Internal keyboard error */
#define KBDMSG_ERROR_ALT 0xFF /* Alternate error message */

#define KBD_FLAG_KEYPAD_KEY  0x40
#define KBD_KEYSTATE_PRESSED 0x80

#define KBD_LED_SCROLL_LOCK 0x01
#define KBD_LED_NUM_LOCK    0x02
#define KBD_LED_CAPS_LOCK   0x04

#define KBD_STATUS_OUTPUT   0x01 /* output buffer full */
#define KBD_STATUS_INPUT    0x02 /* input buffer full */
#define KBD_STATUS_SYSTEM   0x04
#define KBD_STATUS_CMD_DATA 0x08
#define KBD_STATUS_LOCKED   0x10
#define KBD_STATUS_AUX_BUF  0x20
#define KBD_STATUS_TIMEOUT  0x40
#define KBD_STATUS_PARITY   0x80

typedef struct key_info {
    uint8_t scan_code;
    char character;
    bool pressed;
} key_info_t;

typedef void (* keyboard_event_handler_t)(key_info_t *key_info);

void kbd_init(void);
uint8_t read_status_register(void);
uint8_t kbd_read_encoder_buffer(void);
void kbd_send_control_command(uint8_t packet);
void kbd_send_encoder_command(uint8_t packet);
void kbd_set_leds(bool caps_lock, bool num_lock, bool scroll_lock);

#endif /* INCLUDE_IO_KEYBOARD_KEYBOARD_H_ */
