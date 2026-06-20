#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

enum class Color : uint32_t {
    Black        = 0x00000000,
    DarkGray     = 0x004A4A4A,
    Blue         = 0x001F618D,
    Green        = 0x0027AE60,
    Cyan         = 0x002980B9,
    Red          = 0x00C0392B,
    Magenta      = 0x008E44AD,
    Brown        = 0x007E5109,
    White        = 0x00E5E7E9,
    LightBlue    = 0x003498DB,
    LightGreen   = 0x002ECC71,
    LightCyan    = 0x00AED6F1,
    LightRed     = 0x00E74C3C,
    LightMagenta = 0x00BB8FCE,
    Yellow       = 0x00F4D03F,
    BrightWhite  = 0x00FFFFFF,

    Charcoal     = 0x002C3E50,
    Silver       = 0x00BDC3C7,
    Orange       = 0x00E67E22,
    DeepOrange   = 0x00D35400,
    Pink         = 0x00F48FB1,
    HotPink      = 0x00E91E63,
    Teal         = 0x00117A65,
    Mint         = 0x00A3E4D7,
    Gold         = 0x00F1C40F,
    Salmon       = 0x00F1948A,
    Indigo       = 0x004B0082,
    Lime         = 0x0032CD32,
    Coral        = 0x00FF7F50,
    Crimson      = 0x00DC143C,
    Lavender     = 0x00B19CD9,
    Olive        = 0x00808000,

    Transparent  = 0x00000000
};

#endif // COLOR_H
