#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <io/keyboard/keyboard.h>
#include <io/8042/ps2.h>

void kbd_initialize(void) {
    if(!kbd_reset()) {
        printf("kbd_initialize: reset failed.\n");
    }
}

bool kbd_reset(void) {
    ps2_data_t data;

    ps2_send_io(KBDCMD_INIT);
    ps2_wait_for_data(&data);

    if(data.timeout) {
        printf("kbd_reset: timeout while initializing. giving up.\n");
        return false;
    }

    if(data.response == PS2_DEVRESPONSE_RESEND) {
        ps2_resend_last();
    } else if(data.response == PS2_DEVRESPONSE_ACK) {
        ps2_wait_for_data(&data);
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
