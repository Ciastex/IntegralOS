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

#define VK_ESCAPE    0x01
#define VK_ALPHA1    0x02
#define VK_ALPHA2    0x03
#define VK_ALPHA3    0x04
#define VK_ALPHA4    0x05
#define VK_ALPHA5    0x06
#define VK_ALPHA6    0x07
#define VK_ALPHA7    0x08
#define VK_ALPHA8    0x09
#define VK_ALPHA9    0x0A
#define VK_ALPHA0    0x0B
#define VK_MINUS     0x0C
#define VK_EQUALS    0x0D
#define VK_BACKSPACE 0x0E
#define VK_TAB       0x0F
#define VK_Q         0x10
#define VK_W         0x11
#define VK_E         0x12
#define VK_R         0x13
#define VK_T         0x14
#define VK_Y         0x15
#define VK_U         0x16
#define VK_I         0x17
#define VK_O         0x18
#define VK_P         0x19
#define VK_A         0x1E
#define VK_S         0x1F
#define VK_D         0x20
#define VK_F         0x21
#define VK_G         0x22
#define VK_H         0x23
#define VK_J         0x24
#define VK_K         0x25
#define VK_L         0x26
#define VK_Z         0x2C
#define VK_X         0x2D
#define VK_C         0x2E
#define VK_V         0x2F
#define VK_B         0x30
#define VK_N         0x31
#define VK_M         0x32
#define VK_BACKTICK  0x29
#define VK_SEMICOLON 0x27
#define VK_QUOTE     0x28
#define VK_BACKSLASH 0x2B
#define VK_COMMA     0x33
#define VK_PERIOD    0x34
#define VK_SLASH     0x35
#define VK_LBRACKET  0x1A
#define VK_RBRACKET  0x1B
#define VK_SPACE     0x39
#define VK_RETURN    0x1C
#define VK_CAPSLOCK  0x3A
#define VK_LSHIFT    0x2A
#define VK_RSHIFT    0x36
#define VK_CONTROL   0x1D
#define VK_ALT       0x38
#define VK_META      0x5B
#define VK_LEFT      0x4B
#define VK_UP        0x48
#define VK_DOWN      0x50
#define VK_RIGHT     0x4D
#define VK_DELETE    0x53
#define VK_END       0x4F
#define VK_PGDOWN    0x51
#define VK_PGUP      0x49
#define VK_HOME      0x47
#define VK_INSERT    0x52
#define VK_F1        0x3B
#define VK_F2        0x3C
#define VK_F3        0x3D
#define VK_F4        0x3E
#define VK_F5        0x3F
#define VK_F6        0x40
#define VK_F7        0x41
#define VK_F8        0x42
#define VK_F9        0x43
#define VK_F10       0x44
#define VK_F11       0x57
#define VK_F12       0x58

typedef struct key_info {
    uint8_t scan_code;
    uint8_t key_code;
    uint8_t character;
    bool pressed;
} key_info_t;

typedef void (* keyboard_event_handler_t)(key_info_t key_info);

void kbd_init(void);
uint8_t read_status_register(void);
uint8_t kbd_read_encoder_buffer(void);
void kbd_send_control_command(uint8_t packet);
void kbd_send_encoder_command(uint8_t packet);
void kbd_set_pressed_callback(keyboard_event_handler_t handler);
void kbd_set_released_callback(keyboard_event_handler_t handler);
void kbd_set_leds(bool caps_lock, bool num_lock, bool scroll_lock);
bool kbd_is_ctrl_pressed(void);
bool kbd_is_alt_pressed(void);
bool kbd_is_meta_pressed(void);
bool kbd_is_shift_pressed(void);
bool kbd_is_capslock_active(void);

#endif /* INCLUDE_IO_KEYBOARD_KEYBOARD_H_ */
