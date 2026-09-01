// File Updated with AI / Assisted by AI
#include "keymap.h"
#include "config.h"
#include "keyboard.h"

#ifdef KEYBOARD_LAYOUT_DE
const KeyboardLayout current_layout = LAYOUT_DE;
#endif // KEYBOARD_LAYOUT_DE

#ifdef KEYBOARD_LAYOUT_US
const KeyboardLayout current_layout = LAYOUT_US;
#endif // KEYBOARD_LAYOUT_US


static uint16_t extended_scancode_to_keycode(const uint8_t scancode) {
    switch (scancode) {
        case 0x48: return KEY_ARROW_UP;
        case 0x50: return KEY_ARROW_DOWN;
        case 0x4B: return KEY_ARROW_LEFT;
        case 0x4D: return KEY_ARROW_RIGHT;
        case 0x47: return KEY_HOME;
        case 0x4F: return KEY_END;
        case 0x49: return KEY_PAGE_UP;
        case 0x51: return KEY_PAGE_DOWN;
        case 0x52: return KEY_INSERT;
        case 0x53: return KEY_DELETE;
        case 0x5B: return KEY_LGUI;
        case 0x5C: return KEY_RGUI;
        case 0x5D: return KEY_MENU;

        // numpad
        case 0x1C: return KEY_ENTER;
        case 0x35: return '/';
        case 0x1D: return KEY_RCTRL;
        case 0x38: return KEY_RALT;

        default: return KEY_UNKNOWN;
    }
}

uint16_t scancode_to_keycode(const uint8_t scancode, const bool extended) {
    if (extended) return extended_scancode_to_keycode(scancode);

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
