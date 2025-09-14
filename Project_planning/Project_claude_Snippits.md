# led_controller code snippit

## led_controller.h

```cpp
// =============================================================================
// CONSTANTS - These define the structure of the F1's LED output reports
// =============================================================================
// ... Byte positions in the LED report for different LED groups ...

// =============================================================================
// COLOR SYSTEM - All available colors with BRG values
// =============================================================================
// ... color structure ...

// =============================================================================
// STATE STORAGE SYSTEM - NEW! Parallel storage for original LED states
// =============================================================================

/*
* LED State Structure - Stores original color and brightness values
* 
* This structure preserves the original color and brightness values that were
* passed to the LED functions, before they get converted to 7-bit hardware values.
* This allows to restore exact original states after toggling.
* 
* For matrix buttons (LEDStateMatrix): Both color and brightness are meaningful
* For other buttons (LEDState): Only brightness is meaningful (only one color)
*/
struct LEDStateMatrix {
    LEDColor color;      // Original color
    float brightness;    // Original brightness
};

struct LEDState {
        float brightness;    // Original brightness
};

// =============================================================================
// FUNCTION DECLARATIONS - Functions are provided to other files
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


// =============================================================================
// STATE STORAGE FUNCTIONS - Access to original LED states
// =============================================================================

/*
* State Storage Access Functions
* 
* These functions allow other modules (like the toggle system) to access
* the original color and brightness values that were set for each LED,
* before conversion to 7-bit hardware format.
*/

// Get index for special button enum (maps enum to array index)
int getSpecialButtonIndex(SpecialLEDButton button);

// Get index for stop button enum (maps enum to array index)
int getStopButtonIndex(StopLEDButton button);

// Get original state for matrix buttons
LEDStateMatrix getMatrixButtonState(int row, int col);

// Get original state for special buttons  
LEDState getSpecialButtonState(SpecialLEDButton button);

// Get original state for stop buttons
LEDState getStopButtonState(StopLEDButton button);


// =============================================================================
// UTILITY FUNCTIONS - Testing and Debugging
// =============================================================================

void printLEDReport();
void testAllLEDs();
void printLEDStates();

```

## led_controller.cpp

```cpp

#include "headers/led_controller.h"      // Include header file

#include <iostream>             // For std::cout and std::cerr
#include <iomanip>              // For std::hex (hexadecimal printing)
#include <cstring>              // For memset (clearing memory)
#include <unistd.h>             // For usleep (sleep function)
// #include <hidapi/hidapi.h>   // included already in header


// =============================================================================
// GLOBAL LED STATE BYTE BUFFER - Persistent byte buffer for all LED states
// =============================================================================

static unsigned char led_buffer[LED_REPORT_SIZE];
static hid_device* current_device = nullptr;  // Store device for automatic sending

// =============================================================================
// PARALLEL STATE STORAGE - Preserve original color/brightness values
// =============================================================================

/*
* Parallel state storage arrays
* 
* These arrays store the original color and brightness values that were passed
* to the LED functions, BEFORE conversion to 7-bit hardware format. This allows
* other modules, such as the toggle system, to restore exact original values.
*/
static LEDStateMatrix matrix_states[4][4];         // Original states for 4x4 matrix
static LEDState special_states[8];                 // Original states for 8 special buttons
static LEDState stop_states[4];                    // Original states for 4 stop buttons

// ...

// =============================================================================
// STATE STORAGE ACCESS FUNCTIONS - Map button enums to array indices
// =============================================================================

// =======================================
// Get button index functions
// =======================================

int getSpecialButtonIndex(SpecialLEDButton button) {
  // ...
}
int getStopButtonIndex(StopLEDButton button) {
  // ...
}

// =======================================
// Get button state functions
// =======================================

LEDStateMatrix getMatrixButtonState(int row, int col) {
  // ...
}
LEDState getSpecialButtonState(SpecialLEDButton button) {
  // ...
}

// ...

// =============================================================================
// MAIN LED SYSTEM FUNCTIONS
// =============================================================================

/*
* Initializes the LED controller system:
* 1. Stores the device for automatic sending
* 2. Initializes the LED buffer to default values (sets all LEDs to clear)
* 3. Initializes the state storage arrays to default values
*
* @param device: Pointer to the opened HID device
* @return: true if initialization successful, false if error
*/
bool initializeLEDController(hid_device* device) {
    // Step 1: Check if device is valid
    if (device == nullptr) {
        std::cerr << "Error: Device is null in initializeLEDController()" << std::endl;
        return false;
    }
    
    // Step 2: Store device for automatic sending
    current_device = device;
    
    // Step 3: Initialize LED buffer to all zeros (all LEDs off)
    memset(led_buffer, 0, LED_REPORT_SIZE);
    
    // Step 4: Set the report ID (first byte must be 0x80)
    led_buffer[0] = LED_REPORT_ID;

    // Step 5: Initialize state storage arrays to default values
    // MATRIX:Initialize matrix states to black/off
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            matrix_states[row][col] = {LEDColor::black, 0.0f};
        }
    }
    // SPECIAL BUTTONS: Initialize special button states to off (color irrelevant for special buttons)
    for (int i = 0; i < 8; i++) {
        special_states[i] = {0.0f};
    }
    // STOP BUTTONS: Initialize stop button states to off (color irrelevant for stop buttons)
    for (int i = 0; i < 4; i++) {
        stop_states[i] = {0.0f};
    }

    // Step 6: Send initial empty report to turn off all LEDs
    bool success = sendLEDReport(device);
    
    if (success) {
        std::cout << "  - LED controller initialized successfully - all LEDs off, state storage ready" << std::endl;
    } else {
        std::cerr << "Error: Failed to send initial LED report" << std::endl;
    }
    
    return success;
}

bool sendLEDReport(hid_device* device) {
  // ...
}

// =============================================================================
// MATRIX LED FUNCTIONS - Control the 4x4 RGB button grid
// =============================================================================

/*
* Sets a matrix button LED to a specific color and brightness
* Saves the original color and brightness in the state storage
* Matrix buttons are arranged in a 4x4 grid with RGB LEDs (BRG format)
* 
* @param row: Matrix row (1-4)
* @param col: Matrix column (1-4)
* @param color: Color to set (using LEDColor enum)
* @param brightness: Brightness level (0.0 = off, 1.0 = full brightness)
* @return: true if successful, false if error
*/
bool setMatrixButtonLED(int row, int col, LEDColor color, float brightness) {
  // ...
}

// =============================================================================
// SPECIAL BUTTON LED FUNCTIONS - Control single-brightness special buttons
// =============================================================================

/*
* Sets a special button LED to a specific brightness
* Saves the original brightness in state storage
* Special buttons only have single LEDs, so only brightness is controlled
* 
* @param button: Which special button to control (using enum)
* @param brightness: Brightness level (0.0 = off, 1.0 = full brightness)
* @return: true if successful, false if error
*/
bool setSpecialButtonLED(SpecialLEDButton button, float brightness) {
    // Step 1: Get array index for this button
    int index = getSpecialButtonIndex(button);
    if (index < 0 || index > 7) {
        std::cerr << "Error: Invalid special button in setSpecialButtonLED()" << std::endl;
        return false;
    }

    // Step 2: Clamp brightness to valid range
    if (brightness < 0.0f) brightness = 0.0f;
    if (brightness > 1.0f) brightness = 1.0f;

    // Step 3: Save the original brightness in state storage
    special_states[index] = {brightness};

    // Step 4: Convert brightness to 7-bit value (F1 hardware requirement)
    unsigned char led_value = convertTo7Bit(127, brightness);  // Use max 7-bit value with brightness

    // ...
}
```
