#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <hidapi/hidapi.h>

// =============================================================================
// CONSTANTS - These define the structure of the F1's LED output reports
// =============================================================================

// LED output report structure
const int LED_REPORT_SIZE = 81;              // F1 always expects 81-byte LED reports
const unsigned char LED_REPORT_ID = 0x80;    // First byte is always 0x80

// Byte positions in the LED report for different LED groups
const int LED_BYTE_7SEG_RIGHT_START = 1;     // Right 7-segment display (bytes 1-8)
const int LED_BYTE_7SEG_LEFT_START = 9;      // Left 7-segment display (bytes 9-16)
const int LED_BYTE_SPECIAL_START = 17;       // Special buttons (bytes 17-24)
const int LED_BYTE_MATRIX_START = 25;        // RGB matrix buttons (bytes 25-72)
const int LED_BYTE_STOP_START = 73;          // Stop buttons (bytes 73-80)

// Special button byte offsets (from LED_BYTE_SPECIAL_START)
const int LED_OFFSET_BROWSE = 0;     // Byte 17
const int LED_OFFSET_SIZE = 1;       // Byte 18  
const int LED_OFFSET_TYPE = 2;       // Byte 19
const int LED_OFFSET_REVERSE = 3;    // Byte 20
const int LED_OFFSET_SHIFT = 4;      // Byte 21
const int LED_OFFSET_CAPTURE = 5;    // Byte 22
const int LED_OFFSET_QUANT = 6;      // Byte 23
const int LED_OFFSET_SYNC = 7;       // Byte 24

// Stop button byte offsets (from LED_BYTE_STOP_START)
const int LED_OFFSET_STOP4_RIGHT = 0;    // Byte 73
const int LED_OFFSET_STOP4_LEFT = 1;     // Byte 74
const int LED_OFFSET_STOP3_RIGHT = 2;    // Byte 75
const int LED_OFFSET_STOP3_LEFT = 3;     // Byte 76
const int LED_OFFSET_STOP2_RIGHT = 4;    // Byte 77
const int LED_OFFSET_STOP2_LEFT = 5;     // Byte 78
const int LED_OFFSET_STOP1_RIGHT = 6;    // Byte 79
const int LED_OFFSET_STOP1_LEFT = 7;     // Byte 80

// Matrix LED calculation constants
const int MATRIX_LEDS_PER_BUTTON = 3;    // Each matrix button has 3 LEDs (B, R, G)
const int MATRIX_ROWS = 4;               // 4 rows in the matrix
const int MATRIX_COLS = 4;               // 4 columns in the matrix

// =============================================================================
// COLOR SYSTEM - All available colors with BRG values
// =============================================================================

// Color structure for BRG format (hardware requirement)
struct BRGColor {
    unsigned char blue;
    unsigned char red;
    unsigned char green;
};

// Available colors enum - makes code more readable
enum class LEDColor {
    black,        // NEW: Off/no color (0,0,0)
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
    white
};

// Special buttons enum (matches input_reader structure)
enum class SpecialLEDButton {
    BROWSE,
    SIZE,
    TYPE,
    REVERSE,
    SHIFT,
    CAPTURE,
    QUANT,
    SYNC
};

// Stop buttons enum (matches input_reader structure) 
enum class StopLEDButton {
    STOP1 = 1,
    STOP2 = 2,
    STOP3 = 3,
    STOP4 = 4
};

// =============================================================================
// FUNCTION DECLARATIONS - What functions are provided to other files
// =============================================================================

// Main LED system functions
bool initializeLEDController(hid_device* device);
bool sendLEDReport(hid_device* device);
void clearAllLEDs();

// Matrix LED functions (RGB buttons)
bool setMatrixButtonLED(int row, int col, LEDColor color, float brightness);
bool turnOffMatrixButtonLED(int row, int col);

// Special button LED functions (single brightness)  
bool setSpecialButtonLED(SpecialLEDButton button, float brightness);
bool turnOffSpecialButtonLED(SpecialLEDButton button);

// Stop button LED functions (each stop has 2 LEDs)
bool setStopButtonLED(StopLEDButton button, float brightness);
bool turnOffStopButtonLED(StopLEDButton button);

// Color system functions
BRGColor getColorWithBrightness(LEDColor color, float brightness);

// Utility functions for testing and debugging
void printLEDReport();
void testAllLEDs();



#endif // LED_CONTROLLER_H