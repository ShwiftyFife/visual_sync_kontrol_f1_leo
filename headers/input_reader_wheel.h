#ifndef INPUT_READER_WHEEL_H
#define INPUT_READER_WHEEL_H

#include <hidapi/hidapi.h>

// =============================================================================
// CONSTANTS - Wheel input configuration
// =============================================================================

const int WHEEL_BYTE_POSITION = 5;          // Byte 5 contains wheel value (0-255)
const unsigned char WHEEL_MIN_VALUE = 0x00;    // Minimum wheel value
const unsigned char WHEEL_MAX_VALUE = 0xFF;    // Maximum wheel value

// Sensitivity settings
const int WHEEL_ROTATION_THRESHOLD = 1;    // Minimum change to register as rotation
const int WHEEL_WRAP_THRESHOLD = 200;     // Threshold to detect wrap-around (0xFF->0x00 or 0x00->0xFF)

// =============================================================================
// ENUMS - Wheel rotation direction
// =============================================================================

enum class WheelDirection {
    NONE,              // No rotation detected
    CLOCKWISE,         // Rotating clockwise (incrementing)
    COUNTER_CLOCKWISE  // Rotating counter-clockwise (decrementing)
};


// =============================================================================
// WHEEL INPUT READER CLASS
// =============================================================================

class WheelInputReader {
private:
    unsigned char last_wheel_value;     // Previous wheel value for change detection
    bool initialized;                   // Track if we have a baseline value
    
public:
    // Initialization
    bool initialize();
    
    // Main functions
    void updateWheelState(const unsigned char* buffer);
    WheelDirection getWheelDirection(const unsigned char* buffer);
    
    // Utility functions
    void printWheelDebugInfo(const unsigned char* buffer) const;
};

#endif // INPUT_READER_WHEEL_H