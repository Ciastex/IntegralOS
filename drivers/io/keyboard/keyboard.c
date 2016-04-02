/*
 * File name: keyboard.c
 * Description: Keyboard driver implementation.
 *  
 * * * * */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <io/port_io.h>
#include <hal/irq.h>
#include <io/keyboard/keyboard.h>

static void kbd_internal_callback(irq_info_t *irq_info);
static uint8_t kbd_translate_scancode_to_keycode(uint8_t scancode);

void kbd_init(void) {
    irq_set_handler(1, (uintptr_t)&kbd_internal_callback);
}

uint8_t kbd_read_status_register(void) {
    return inb(KBD_CONTROL_STATUS_REG);
}

uint8_t kbd_read_encoder_buffer(void) {
    return inb(KBD_ENCODER_INPUT_BUFFER);
}

void kbd_send_control_command(uint8_t packet) {
    while(1) {
        if(!(kbd_read_status_register() & KBD_STATUS_INPUT)) {
            break;
        }
    }
    outb(KBD_CONTROL_COMMAND_REG, packet);
}

void kbd_send_encoder_command(uint8_t packet) {
    while(1) {
        if(!(kbd_read_status_register() & KBD_STATUS_INPUT)) {
            break;
        }
    }
    outb(KBD_ENCODER_COMMAND_PORT, packet);
}

void kbd_set_leds(bool caps_lock, bool num_lock, bool scroll_lock) {
    uint8_t packet = 0;

    if(caps_lock) {
        packet |= KBD_LED_CAPS_LOCK;
    } else {
        CLEAR_BIT(packet, 2);
    }

    if(num_lock) {
        packet |= KBD_LED_NUM_LOCK;
    } else {
        CLEAR_BIT(packet, 1);
    }

    if(scroll_lock) {
        packet |= KBD_LED_SCROLL_LOCK;
    } else {
        CLEAR_BIT(packet, 0);
    }

    kbd_send_encoder_command(0xED);
    kbd_send_encoder_command(packet);
}

static void kbd_internal_callback(irq_info_t *irq_info) {
    if(inb(KBD_ENCODER_INPUT_BUFFER)) {
        uint8_t scancode = inb(KBD_ENCODER_INPUT_BUFFER);

        if(scancode & KBD_KEYSTATE_PRESSED) {
            printf("%08b\n", kbd_translate_scancode_to_keycode(scancode));
        } else {
            //printf("%08b\n", kbd_translate_scancode_to_keycode(scancode));
        }
    }
}

static uint8_t kbd_translate_scancode_to_keycode(uint8_t scancode) {
    return scancode & ~(1 << 7);
}
