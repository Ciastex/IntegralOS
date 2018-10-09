#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <io/keyboard/keyboard.h>
#include <io/8042/ps2.h>
#include <hal/irq.h>

static bool _kbd_send_io(ps2_word_t packet, ps2_data_t* data);
static void _kbd_internal(irq_info_t* irq);

void kbd_initialize(void) {
    if(!kbd_reset()) {
        printf("kbd_initialize: reset failed.\n");
        return;
    }

    kbd_set_scancode_set(SCANCODE_SET_3);
    kbd_set_scanning(true);

    irq_set_handler(1, (uintptr_t)&_kbd_internal);

    ps2_flush_data_port();
}

bool kbd_reset(void) {
    ps2_data_t data;

    if(!_kbd_send_io(KBDCMD_INIT, &data)) {
        if(data.timeout) {
            printf("kbd_reset: timeout while sending command. giving up.\n");
        } else printf("kbd_reset: unexpected response while sending command. giving up.\n");

        return false;
    }

    ps2_wait_for_data(&data);
    if(data.timeout) {
        printf("kbd_reset: timeout while getting test result. giving up.\n");
        return false;
    }

    if(data.response == KBDRESPONSE_TESTFAILED_1 || data.response == KBDRESPONSE_TESTFAILED_2) {
        printf("kbd_reset: init test returned failure.\n");
        return false;
    } else if(data.response != KBDRESPONSE_INIT_OK) {
        printf("kbd_reset: init test returned unexpected value: 0x%02X\n", data.response);
        return false;
    }

    return true;
}

kbd_scancode_set_t kbd_get_current_scancode_set(void) {
    ps2_data_t data;

    if(!_kbd_send_io(KBDCMD_SCANCODES, &data)) {
        if(data.timeout) {
            printf("kbd_get_current_scancode_set: timeout\n");
        } else printf("kbd_get_current_scancode_set: invalid response 0x%02X\n", data.response);

        return SCANCODE_ERROR;
    }

    if(!_kbd_send_io(KBDSCANCODE_GET, &data)) {
        if(data.timeout) {
            printf("kbd_get_current_scancode_set: no scancode received\n");
        } else printf("kbd_get_current_scancode_set: invalid response 0x%02X\n", data.response);

        return SCANCODE_ERROR;
    }

    ps2_wait_for_data(&data);
    if(data.timeout) {
        return SCANCODE_ERROR;
    }

    return (kbd_scancode_set_t)data.response;
}

void kbd_set_scancode_set(kbd_scancode_set_t set) {
    ps2_data_t data;

    if(!_kbd_send_io(KBDCMD_SCANCODES, &data)) {
        if(data.timeout) {
            printf("kbd_set_scancode_set: timeout\n");
        } else printf("kbd_scancode_set: invalid response 0x%02X\n", data.response);

        return;
    }

    if(!_kbd_send_io(set, &data)) {
        if(data.timeout) {
            printf("kbd_set_scancode_set: can't be sure if scancode is set; timeout\n");
        } else printf("kbd_set_scancode_set: invalid response 0x%02X\n", data.response);
    }
}

void kbd_set_scanning(bool scanning) {
    ps2_data_t data;
    ps2_word_t packet = scanning ? PS2_DEVCMD_ENABLE_SCANNING : PS2_DEVCMD_DISABLE_SCANNING;

    if(!_kbd_send_io(packet, &data)) {
        if(data.timeout) {
            printf("kbd_set_scanning: timeout");
        } else printf("kbd_set_scanning: invalid response 0x%02X\n", data.response);
    }
}

static bool _kbd_send_io(ps2_word_t packet, ps2_data_t* data) {
    bool ret = false;
    uint8_t retries = PS2_MAX_RETRIES;

    ps2_send_io(packet);
    ps2_wait_for_data(data);

    if(data->timeout)
        return false;

    if(data->response == PS2_DEVRESPONSE_RESEND) {
        printf("_kbd_send_io: resending 0x%02X\n", packet);

        while(retries--) {
            ps2_resend_last();
            ps2_wait_for_data(data);

            if(data->timeout)
                return false;

            if(data->response != PS2_DEVRESPONSE_RESEND)
                break;
        }

        if(!retries)
            return false;

        if(data->response == PS2_DEVRESPONSE_ACK) {
            ret = true;
        }
    } else if(data->response == PS2_DEVRESPONSE_ACK) {
        ret = true;
    }

    return ret;
}

static void _kbd_internal(irq_info_t* irq) {
    if(ps2_can_read()) {
        bool pressed = true;

        ps2_data_t data;
        ps2_wait_for_data(&data);

        if(data.response == 0xF0) {
            pressed = false;
            ps2_wait_for_data(&data);
        }

        if(pressed)
            printf("press: %08b\n", data.response);
        else printf("release: %08b\n", data.response);
    }
    ps2_flush_data_port();
}
