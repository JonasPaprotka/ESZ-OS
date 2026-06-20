// File Updated with AI / Assisted by AI
#include "config.h"
#include <stdint.h>
#include "keymap.h"
#include "keyboard.h"

#ifdef KEYBOARD_LAYOUT_DE
const KeyboardLayout current_layout = LAYOUT_DE;
#endif // KEYBOARD_LAYOUT_DE

#ifdef KEYBOARD_LAYOUT_US
const KeyboardLayout current_layout = LAYOUT_US;
#endif // KEYBOARD_LAYOUT_US


uint16_t scancode_to_keycode(const uint8_t scancode) {
    if (scancode >= 0x3A) return KEY_UNKNOWN;
    int shift_idx = shift ? 1 : 0;

    if (current_layout == LAYOUT_DE) {
        return de_keymap[shift_idx][scancode];
    }

    return us_keymap[shift_idx][scancode];
}


// en-US
uint16_t us_keymap[2][0x3A] = {
    // Unshifted
    {
        0, KEY_ESCAPE, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', KEY_BACKSPACE,
        KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', KEY_ENTER,
        KEY_LCTRL, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        KEY_LSHIFT, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', KEY_RSHIFT,
        '*', KEY_LALT, ' '
    },
    // Shifted
    {
        0, KEY_ESCAPE, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', KEY_BACKSPACE,
        KEY_TAB, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', KEY_ENTER,
        KEY_LCTRL, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
        KEY_LSHIFT, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', KEY_RSHIFT,
        '*', KEY_LALT, ' '
    }
};

// de-DE
uint16_t de_keymap[2][0x3A] = {
    // Unshifted
    {
        0, KEY_ESCAPE, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', KEY_DE_SS, '\'', KEY_BACKSPACE,
        KEY_TAB, 'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', KEY_DE_UE, '+', KEY_ENTER,
        KEY_LCTRL, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', KEY_DE_OE, KEY_DE_AE, '^',
        KEY_LSHIFT, '#', 'y', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', KEY_RSHIFT,
        '*', KEY_LALT, ' '
    },
    // Shifted
    {
        0, KEY_ESCAPE, '!', '"', '3', '$', '%', '&', '/', '(', ')', '=', '?', '`', KEY_BACKSPACE,
        KEY_TAB, 'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', KEY_DE_UE_CAPS, '*', KEY_ENTER,
        KEY_LCTRL, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', KEY_DE_OE_CAPS, KEY_DE_AE_CAPS, '2',
        KEY_LSHIFT, '\'', 'Y', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', KEY_RSHIFT,
        '*', KEY_LALT, ' '
    }
};
