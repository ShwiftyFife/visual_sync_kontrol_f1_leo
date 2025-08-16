#include "headers/color_system.h"

#include <stdio.h>  // stdio.h gives printf() — basic text output (like print() in R).

// ________________________________________________________________
// FUNCTION: CONVERT to 7-BIT
// Helper function to convert 8-bit color value to 7-bit with brightness
uint8_t convertTo7Bit(uint8_t value_8bit, float brightness) {

    // Convert 8-bit (0-255) to 7-bit (0-127) and apply brightness
    float result = (value_8bit * 127.0f / 255.0f) * brightness;
    
    // Clamp to valid range and round
    if (result < 0.0f) result = 0.0f;
    if (result > 127.0f) result = 127.0f;
    
    return (uint8_t)(result + 0.5f); // +0.5f for proper rounding
}

// ________________________________________________________________
// FUNCTION: GET COLOR WITH BRIGHTNESS
// Get the BRG color values with applied brightness
BRGColor getColorWithBrightness(ColorSystem color, float brightness) {
    // Clamp brightness to valid range
    if (brightness < 0.0f) brightness = 0.0f;
    if (brightness > 1.0f) brightness = 1.0f;
    
    BRGColor result;
    
    // Your color definitions (in 8-bit BRG format)
    switch(color) {
        case ColorSystem::red:
            result.blue = convertTo7Bit(0, brightness);
            result.red = convertTo7Bit(255, brightness);
            result.green = convertTo7Bit(0, brightness);
            break;
            
        case ColorSystem::orange:
            result.blue = convertTo7Bit(45, brightness);
            result.red = convertTo7Bit(255, brightness);
            result.green = convertTo7Bit(97, brightness);
            break;
            
        case ColorSystem::lightorange:
            result.blue = convertTo7Bit(0, brightness);
            result.red = convertTo7Bit(255, brightness);
            result.green = convertTo7Bit(148, brightness);
            break;
            
        case ColorSystem::warmyellow:
            result.blue = convertTo7Bit(0, brightness);
            result.red = convertTo7Bit(255, brightness);
            result.green = convertTo7Bit(213, brightness);
            break;
            
        case ColorSystem::yellow:
            result.blue = convertTo7Bit(0, brightness);
            result.red = convertTo7Bit(255, brightness);
            result.green = convertTo7Bit(255, brightness);
            break;
            
        case ColorSystem::lime:
            result.blue = convertTo7Bit(0, brightness);
            result.red = convertTo7Bit(144, brightness);
            result.green = convertTo7Bit(255, brightness);
            break;
            
        case ColorSystem::green:
            result.blue = convertTo7Bit(0, brightness);
            result.red = convertTo7Bit(0, brightness);
            result.green = convertTo7Bit(255, brightness);
            break;
            
        case ColorSystem::mint:
            result.blue = convertTo7Bit(165, brightness);
            result.red = convertTo7Bit(0, brightness);
            result.green = convertTo7Bit(255, brightness);
            break;
            
        case ColorSystem::cyan:
            result.blue = convertTo7Bit(255, brightness);
            result.red = convertTo7Bit(0, brightness);
            result.green = convertTo7Bit(255, brightness);
            break;
            
        case ColorSystem::turquise:
            result.blue = convertTo7Bit(255, brightness);
            result.red = convertTo7Bit(0, brightness);
            result.green = convertTo7Bit(206, brightness);
            break;
            
        case ColorSystem::blue:
            result.blue = convertTo7Bit(255, brightness);
            result.red = convertTo7Bit(0, brightness);
            result.green = convertTo7Bit(49, brightness);
            break;
            
        case ColorSystem::plum:
            result.blue = convertTo7Bit(218, brightness);
            result.red = convertTo7Bit(69, brightness);
            result.green = convertTo7Bit(49, brightness);
            break;
            
        case ColorSystem::violet:
            result.blue = convertTo7Bit(217, brightness);
            result.red = convertTo7Bit(125, brightness);
            result.green = convertTo7Bit(41, brightness);
            break;
            
        case ColorSystem::purple:
            result.blue = convertTo7Bit(255, brightness);
            result.red = convertTo7Bit(229, brightness);
            result.green = convertTo7Bit(18, brightness);
            break;
            
        case ColorSystem::magenta:
            result.blue = convertTo7Bit(255, brightness);
            result.red = convertTo7Bit(255, brightness);
            result.green = convertTo7Bit(0, brightness);
            break;
            
        case ColorSystem::fuchsia:
            result.blue = convertTo7Bit(136, brightness);
            result.red = convertTo7Bit(255, brightness);
            result.green = convertTo7Bit(0, brightness);
            break;
            
        case ColorSystem::white:
            result.blue = convertTo7Bit(255, brightness);
            result.red = convertTo7Bit(255, brightness);
            result.green = convertTo7Bit(255, brightness);
            break;
        
        case ColorSystem::black:
            result.blue = convertTo7Bit(0, brightness);
            result.red = convertTo7Bit(0, brightness);
            result.green = convertTo7Bit(0, brightness);
            break;
    }
    
    return result;
}