#ifndef INCLUDE_IO_KEYBOARD_KEYMAP_H_
#define INCLUDE_IO_KEYBOARD_KEYMAP_H_

#include <io/keyboard/keycodes.h>

char keymap_unshifted[255] = {
    [VK_A] = 'a', [VK_ALPHA1] = '1',
    [VK_B] = 'b', [VK_ALPHA2] = '2',
    [VK_C] = 'c', [VK_ALPHA3] = '3',
    [VK_D] = 'd', [VK_ALPHA4] = '4',
    [VK_E] = 'e', [VK_ALPHA5] = '5',
    [VK_F] = 'f', [VK_ALPHA6] = '6',
    [VK_G] = 'g', [VK_ALPHA7] = '7',
    [VK_H] = 'h', [VK_ALPHA8] = '8',
    [VK_I] = 'i', [VK_ALPHA9] = '9',
    [VK_J] = 'j', [VK_ALPHA0] = '0',
    [VK_K] = 'k', [VK_TILDE]  = '`',
    [VK_L] = 'l', [VK_MINUS]  = '-',
    [VK_M] = 'm', [VK_EQUALS] = '=',
    [VK_N] = 'n', [VK_LBRACK] = '[',
    [VK_O] = 'o', [VK_RBRACK] = ']',
    [VK_P] = 'p', [VK_SEMCOL] = ';',
    [VK_Q] = 'q', [VK_QUOTE]  = '\'',
    [VK_R] = 'r', [VK_BKSLSH] = '\\',
    [VK_S] = 's', [VK_COMMA]  = ',',
    [VK_T] = 't', [VK_PERIOD] = '.',
    [VK_U] = 'u', [VK_SLASH]  = '/',
    [VK_V] = 'v', [VK_SPACE]  = ' ',
    [VK_W] = 'w',
    [VK_X] = 'x',
    [VK_Y] = 'y',
    [VK_Z] = 'z',
};

char keymap_shifted[255] = {
    [VK_A] = 'A', [VK_ALPHA1] = '!',
    [VK_B] = 'B', [VK_ALPHA2] = '@',
    [VK_C] = 'C', [VK_ALPHA3] = '#',
    [VK_D] = 'D', [VK_ALPHA4] = '$',
    [VK_E] = 'E', [VK_ALPHA5] = '%',
    [VK_F] = 'F', [VK_ALPHA6] = '^',
    [VK_G] = 'G', [VK_ALPHA7] = '&',
    [VK_H] = 'H', [VK_ALPHA8] = '*',
    [VK_I] = 'I', [VK_ALPHA9] = '(',
    [VK_J] = 'J', [VK_ALPHA0] = ')',
    [VK_K] = 'K', [VK_TILDE]  = '~',
    [VK_L] = 'L', [VK_MINUS]  = '_',
    [VK_M] = 'M', [VK_EQUALS] = '+',
    [VK_N] = 'N', [VK_LBRACK] = '{',
    [VK_O] = 'O', [VK_RBRACK] = '}',
    [VK_P] = 'P', [VK_SEMCOL] = ':',
    [VK_Q] = 'Q', [VK_QUOTE]  = '"',
    [VK_R] = 'R', [VK_BKSLSH] = '|',
    [VK_S] = 'S', [VK_COMMA]  = '<',
    [VK_T] = 'T', [VK_PERIOD] = '>',
    [VK_U] = 'U', [VK_SLASH]  = '?',
    [VK_V] = 'V', [VK_SPACE]  = ' ',
    [VK_W] = 'W',
    [VK_X] = 'X',
    [VK_Y] = 'Y',
    [VK_Z] = 'Z',
};


#endif // INCLUDE_IO_KEYBOARD_KEYMAP_H_
