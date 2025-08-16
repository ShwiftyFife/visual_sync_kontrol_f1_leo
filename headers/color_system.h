#ifndef COLOR_SYSTEM_H
#define COLOR_SYSTEM_H

#include <stdint.h> // stdint.h gives uint8_t support. uint8_t is an unsigned 8-bit integer type.

enum class ColorSystem {
    red,
    orange,
    lightorange,
    warmyellow,
    yellow,
    lime,
    green,
    mint,
    cyan,
    turquise,
    blue,
    plum,
    violet,
    purple,
    magenta,
    fuchsia,
    white,
    black
};

// Structure to hold the final BRG values (7-bit range: 0-127)
struct BRGColor {
    uint8_t blue;
    uint8_t red;
    uint8_t green;
};

BRGColor getColorWithBrightness(ColorSystem color, float brightness);

#endif