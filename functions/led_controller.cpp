#include "headers/led_controller.h"      // Include header file

#include <iostream>             // For std::cout and std::cerr
#include <iomanip>              // For std::hex (hexadecimal printing)
#include <cstring>              // For memset (clearing memory)
#include <unistd.h>             // For usleep (sleep function)
// #include <hidapi/hidapi.h>   // included already in header


// =============================================================================
// GLOBAL LED STATE BUFFER - Persistent storage for all LED states
// =============================================================================

/*
* This buffer holds the current state of all LEDs on the F1.
* It's persistent, so when you change one LED, all others stay the same.
* The buffer is always ready to send to the F1 device.
*/
static unsigned char led_buffer[LED_REPORT_SIZE];
static hid_device* current_device = nullptr;  // Store device for automatic sending

// =============================================================================
// HELPER FUNCTIONS - Internal functions for color conversion and validation
// =============================================================================

/*
* Converts 8-bit color value (0-255) to 7-bit value (0-127) with brightness
* The F1 hardware only accepts 7-bit color values, so we must convert.
* 
* @param value_8bit: Original color value (0-255)
* @param brightness: Brightness multiplier (0.0 = off, 1.0 = full brightness)
* @return: 7-bit color value (0-127) with brightness applied
*/
static unsigned char convertTo7Bit(unsigned char value_8bit, float brightness) {
    // Step 1: Clamp brightness to valid range (0.0 to 1.0)
    if (brightness < 0.0f) brightness = 0.0f;
    if (brightness > 1.0f) brightness = 1.0f;
    
    // Step 2: Convert 8-bit to 7-bit and apply brightness
    float result = (value_8bit * 127.0f / 255.0f) * brightness;
    
    // Step 3: Clamp result to valid 7-bit range
    if (result < 0.0f) result = 0.0f;
    if (result > 127.0f) result = 127.0f;
    
    // Step 4: Round to nearest integer and return
    return (unsigned char)(result + 0.5f);
}

/*
* Validates matrix button position (row and column)
* Matrix buttons are arranged in a 4x4 grid, rows 1-4, columns 1-4
* 
* @param row: Matrix row (should be 1-4)
* @param col: Matrix column (should be 1-4)  
* @return: true if position is valid, false if invalid
*/
static bool isValidMatrixPosition(int row, int col) {
    return (row >= 1 && row <= MATRIX_ROWS && col >= 1 && col <= MATRIX_COLS);
}

// =============================================================================
// COLOR SYSTEM FUNCTIONS - Convert colors to BRG format
// =============================================================================

/*
* Gets a color in BRG format with specified brightness
* All 18 colors are defined here with their RGB values, then converted to BRG
* 
* @param color: The color to get (using LEDColor enum)
* @param brightness: Brightness level (0.0 = off, 1.0 = full brightness)
* @return: BRGColor structure with blue, red, green values (7-bit each)
*/
BRGColor getColorWithBrightness(LEDColor color, float brightness) {

    // Initialize result color
    BRGColor result;
    
    // Define all colors in RGB format, then convert to BRG with brightness
    switch(color) {
        case LEDColor::black:           // NEW: Off/no color
            result.blue = convertTo7Bit(0, brightness);
            result.red = convertTo7Bit(0, brightness);
            result.green = convertTo7Bit(0, brightness);
            break;
            
        case LEDColor::red:
            result.blue = convertTo7Bit(0, brightness);
            result.red = convertTo7Bit(255, brightness);
            result.green = convertTo7Bit(0, brightness);
            break;
            
        case LEDColor::orange:
            result.blue = convertTo7Bit(45, brightness);
            result.red = convertTo7Bit(255, brightness);
            result.green = convertTo7Bit(97, brightness);
            break;
            
        case LEDColor::lightorange:
            result.blue = convertTo7Bit(0, brightness);
            result.red = convertTo7Bit(255, brightness);
            result.green = convertTo7Bit(148, brightness);
            break;
            
        case LEDColor::warmyellow:
            result.blue = convertTo7Bit(0, brightness);
            result.red = convertTo7Bit(255, brightness);
            result.green = convertTo7Bit(213, brightness);
            break;
            
        case LEDColor::yellow:
            result.blue = convertTo7Bit(0, brightness);
            result.red = convertTo7Bit(255, brightness);
            result.green = convertTo7Bit(255, brightness);
            break;
            
        case LEDColor::lime:
            result.blue = convertTo7Bit(0, brightness);
            result.red = convertTo7Bit(144, brightness);
            result.green = convertTo7Bit(255, brightness);
            break;
            
        case LEDColor::green:
            result.blue = convertTo7Bit(0, brightness);
            result.red = convertTo7Bit(0, brightness);
            result.green = convertTo7Bit(255, brightness);
            break;
            
        case LEDColor::mint:
            result.blue = convertTo7Bit(165, brightness);
            result.red = convertTo7Bit(0, brightness);
            result.green = convertTo7Bit(255, brightness);
            break;
            
        case LEDColor::cyan:
            result.blue = convertTo7Bit(255, brightness);
            result.red = convertTo7Bit(0, brightness);
            result.green = convertTo7Bit(255, brightness);
            break;
            
        case LEDColor::turquise:
            result.blue = convertTo7Bit(255, brightness);
            result.red = convertTo7Bit(0, brightness);
            result.green = convertTo7Bit(206, brightness);
            break;
            
        case LEDColor::blue:
            result.blue = convertTo7Bit(255, brightness);
            result.red = convertTo7Bit(0, brightness);
            result.green = convertTo7Bit(49, brightness);
            break;
            
        case LEDColor::plum:
            result.blue = convertTo7Bit(218, brightness);
            result.red = convertTo7Bit(69, brightness);
            result.green = convertTo7Bit(49, brightness);
            break;
            
        case LEDColor::violet:
            result.blue = convertTo7Bit(217, brightness);
            result.red = convertTo7Bit(125, brightness);
            result.green = convertTo7Bit(41, brightness);
            break;
            
        case LEDColor::purple:
            result.blue = convertTo7Bit(255, brightness);
            result.red = convertTo7Bit(229, brightness);
            result.green = convertTo7Bit(18, brightness);
            break;
            
        case LEDColor::magenta:
            result.blue = convertTo7Bit(255, brightness);
            result.red = convertTo7Bit(255, brightness);
            result.green = convertTo7Bit(0, brightness);
            break;
            
        case LEDColor::fuchsia:
            result.blue = convertTo7Bit(136, brightness);
            result.red = convertTo7Bit(255, brightness);
            result.green = convertTo7Bit(0, brightness);
            break;
            
        case LEDColor::white:
            result.blue = convertTo7Bit(255, brightness);
            result.red = convertTo7Bit(255, brightness);
            result.green = convertTo7Bit(255, brightness);
            break;
    }
    
    return result;
}

// =============================================================================
// MAIN LED SYSTEM FUNCTIONS
// =============================================================================

/*
* Initializes the LED controller system and clears all LEDs
* This sets up the LED buffer and stores the device for automatic sending
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
    
    // Step 5: Send initial empty report to turn off all LEDs
    bool success = sendLEDReport(device);
    
    if (success) {
        std::cout << "LED controller initialized successfully - all LEDs off" << std::endl;
    } else {
        std::cerr << "Error: Failed to send initial LED report" << std::endl;
    }
    
    return success;
}

/*
* Sends the current LED buffer to the F1 device
* This function actually communicates with the hardware
* 
* @param device: Pointer to the opened HID device
* @return: true if send successful, false if error
*/
bool sendLEDReport(hid_device* device) {
    // Step 1: Check if device is valid
    if (device == nullptr) {
        std::cerr << "Error: Device is null in sendLEDReport()" << std::endl;
        return false;
    }
    
    // Step 2: Send the 81-byte LED report to the F1
    int bytes_sent = hid_write(device, led_buffer, LED_REPORT_SIZE);
    
    // Step 3: Check if the send operation was successful
    if (bytes_sent < 0) {
        std::cerr << "Error: Failed to send LED report. Expected " 
                  << LED_REPORT_SIZE << " bytes, sent " << bytes_sent << " bytes" << std::endl;
        return false;
    }
    
    // Step 4: Verify correct number of bytes were sent
    if (bytes_sent != LED_REPORT_SIZE) {
        std::cerr << "Warning: Partial LED report sent. Expected " 
                  << LED_REPORT_SIZE << " bytes, sent " << bytes_sent << " bytes" << std::endl;
        return false;
    }
    
    // Step 5: Success!
    return true;
}

/*
* Clears all LEDs (turns them off) and sends the update to the F1
* This is useful for resetting the LED state
*/
void clearAllLEDs() {
    // Step 1: Clear the entire buffer except the report ID
    memset(led_buffer + 1, 0, LED_REPORT_SIZE - 1);  // Skip first byte (report ID)
    
    // Step 2: Send the cleared buffer to the F1
    if (current_device != nullptr) {
        sendLEDReport(current_device);
        std::cout << "All LEDs cleared" << std::endl;
    } else {
        std::cerr << "Warning: No device connected, LEDs cleared in buffer only" << std::endl;
    }
}

// =============================================================================
// MATRIX LED FUNCTIONS - Control the 4x4 RGB button grid
// =============================================================================

/*
* Sets a matrix button LED to a specific color and brightness
* Matrix buttons are arranged in a 4x4 grid with RGB LEDs (BRG format)
* 
* @param row: Matrix row (1-4)
* @param col: Matrix column (1-4)
* @param color: Color to set (using LEDColor enum)
* @param brightness: Brightness level (0.0 = off, 1.0 = full brightness)
* @return: true if successful, false if error
*/
bool setMatrixButtonLED(int row, int col, LEDColor color, float brightness) {
    // Step 1: Validate matrix position
    if (!isValidMatrixPosition(row, col)) {
        std::cerr << "Error: Invalid matrix position (" << row << "," << col 
                  << "). Must be 1-4 for both row and column." << std::endl;
        return false;
    }
    
    // Step 2: Calculate the byte position for this matrix button
    // Matrix mapping: (row-1) * 4 + (col-1) gives button index (0-15)
    // Each button has 3 bytes (BRG), so multiply by 3
    int button_index = (row - 1) * MATRIX_COLS + (col - 1);
    int base_byte = LED_BYTE_MATRIX_START + (button_index * MATRIX_LEDS_PER_BUTTON);
    
    // Step 3: Get the color values in BRG format with brightness
    BRGColor brg_color = getColorWithBrightness(color, brightness);
    
    // Step 4: Set the three LED bytes for this button (Blue, Red, Green order)
    led_buffer[base_byte]     = brg_color.blue;   // Blue LED
    led_buffer[base_byte + 1] = brg_color.red;    // Red LED  
    led_buffer[base_byte + 2] = brg_color.green;  // Green LED
    
    // Step 5: Send the updated buffer to the F1
    if (current_device != nullptr) {
        return sendLEDReport(current_device);
    } else {
        std::cerr << "Warning: No device connected, LED set in buffer only" << std::endl;
        return false;
    }
}

/*
* Turns off a specific matrix button LED
* This is a convenience function equivalent to setting black color
* 
* @param row: Matrix row (1-4)
* @param col: Matrix column (1-4)  
* @return: true if successful, false if error
*/
bool turnOffMatrixButtonLED(int row, int col) {
    return setMatrixButtonLED(row, col, LEDColor::black, 0.0f);
}

// =============================================================================
// SPECIAL BUTTON LED FUNCTIONS - Control single-brightness special buttons
// =============================================================================

/*
* Sets a special button LED to a specific brightness
* Special buttons only have single LEDs, so only brightness is controlled
* 
* @param button: Which special button to control (using enum)
* @param brightness: Brightness level (0.0 = off, 1.0 = full brightness)
* @return: true if successful, false if error
*/
bool setSpecialButtonLED(SpecialLEDButton button, float brightness) {
    // Step 1: Clamp brightness to valid range
    if (brightness < 0.0f) brightness = 0.0f;
    if (brightness > 1.0f) brightness = 1.0f;
    
    // Step 2: Convert brightness to 7-bit value (F1 hardware requirement)
    unsigned char led_value = convertTo7Bit(127, brightness);  // Use max 7-bit value with brightness
    
    // Step 3: Determine which byte to set based on the button
    int byte_position;
    
    switch (button) {
        case SpecialLEDButton::BROWSE:
            byte_position = LED_BYTE_SPECIAL_START + LED_OFFSET_BROWSE;
            break;
        case SpecialLEDButton::SIZE:
            byte_position = LED_BYTE_SPECIAL_START + LED_OFFSET_SIZE;
            break;
        case SpecialLEDButton::TYPE:
            byte_position = LED_BYTE_SPECIAL_START + LED_OFFSET_TYPE;
            break;
        case SpecialLEDButton::REVERSE:
            byte_position = LED_BYTE_SPECIAL_START + LED_OFFSET_REVERSE;
            break;
        case SpecialLEDButton::SHIFT:
            byte_position = LED_BYTE_SPECIAL_START + LED_OFFSET_SHIFT;
            break;
        case SpecialLEDButton::CAPTURE:
            byte_position = LED_BYTE_SPECIAL_START + LED_OFFSET_CAPTURE;
            break;
        case SpecialLEDButton::QUANT:
            byte_position = LED_BYTE_SPECIAL_START + LED_OFFSET_QUANT;
            break;
        case SpecialLEDButton::SYNC:
            byte_position = LED_BYTE_SPECIAL_START + LED_OFFSET_SYNC;
            break;
        default:
            std::cerr << "Error: Unknown special button requested" << std::endl;
            return false;
    }
    
    // Step 4: Set the LED value in the buffer
    led_buffer[byte_position] = led_value;
    
    // Step 5: Send the updated buffer to the F1
    if (current_device != nullptr) {
        return sendLEDReport(current_device);
    } else {
        std::cerr << "Warning: No device connected, LED set in buffer only" << std::endl;
        return false;
    }
}

/*
* Turns off a specific special button LED
* This is a convenience function equivalent to setting brightness to 0
* 
* @param button: Which special button to turn off (using enum)
* @return: true if successful, false if error
*/
bool turnOffSpecialButtonLED(SpecialLEDButton button) {
    return setSpecialButtonLED(button, 0.0f);
}

// =============================================================================
// STOP BUTTON LED FUNCTIONS - Control stop button LEDs (2 LEDs per button)
// =============================================================================

/*
* Sets both LEDs of a stop button to a specific brightness
* Each stop button has 2 separate LEDs (left and right)
* 
* @param button: Which stop button to control (STOP1-STOP4)
* @param brightness: Brightness level (0.0 = off, 1.0 = full brightness)
* @return: true if successful, false if error
*/
bool setStopButtonLED(StopLEDButton button, float brightness) {
    // Step 1: Validate stop button number
    if (button < StopLEDButton::STOP1 || button > StopLEDButton::STOP4) {
        std::cerr << "Error: Invalid stop button. Must be STOP1-STOP4." << std::endl;
        return false;
    }
    
    // Step 2: Clamp brightness to valid range
    if (brightness < 0.0f) brightness = 0.0f;
    if (brightness > 1.0f) brightness = 1.0f;
    
    // Step 3: Convert brightness to 7-bit value
    unsigned char led_value = convertTo7Bit(127, brightness);
    
    // Step 4: Determine byte positions for both LEDs of this stop button
    int left_byte, right_byte;
    
    switch (button) {
        case StopLEDButton::STOP1:
            right_byte = LED_BYTE_STOP_START + LED_OFFSET_STOP1_RIGHT;
            left_byte = LED_BYTE_STOP_START + LED_OFFSET_STOP1_LEFT;
            break;
        case StopLEDButton::STOP2:
            right_byte = LED_BYTE_STOP_START + LED_OFFSET_STOP2_RIGHT;
            left_byte = LED_BYTE_STOP_START + LED_OFFSET_STOP2_LEFT;
            break;
        case StopLEDButton::STOP3:
            right_byte = LED_BYTE_STOP_START + LED_OFFSET_STOP3_RIGHT;
            left_byte = LED_BYTE_STOP_START + LED_OFFSET_STOP3_LEFT;
            break;
        case StopLEDButton::STOP4:
            right_byte = LED_BYTE_STOP_START + LED_OFFSET_STOP4_RIGHT;
            left_byte = LED_BYTE_STOP_START + LED_OFFSET_STOP4_LEFT;
            break;
    }
    
    // Step 5: Set both LED values in the buffer
    led_buffer[right_byte] = led_value;
    led_buffer[left_byte] = led_value;
    
    // Step 6: Send the updated buffer to the F1
    if (current_device != nullptr) {
        return sendLEDReport(current_device);
    } else {
        std::cerr << "Warning: No device connected, LED set in buffer only" << std::endl;
        return false;
    }
}

/*
* Turns off both LEDs of a specific stop button
* This is a convenience function equivalent to setting brightness to 0
* 
* @param button: Which stop button to turn off (STOP1-STOP4)
* @return: true if successful, false if error
*/
bool turnOffStopButtonLED(StopLEDButton button) {
    return setStopButtonLED(button, 0.0f);
}

// =============================================================================
// UTILITY FUNCTIONS FOR DEBUGGING
// =============================================================================

/*
* Prints the current LED report buffer in hexadecimal format
* Useful for debugging and understanding what will be sent to the F1
*/
void printLEDReport() {
    std::cout << "Current LED Report (81 bytes):" << std::endl;
    
    // Print report ID
    std::cout << "Report ID: 0x" << std::hex << std::setfill('0') << std::setw(2) 
              << (int)led_buffer[0] << std::endl;
    
    // Print in groups for easier reading
    std::cout << "7-Seg Right (1-8):   ";
    for (int i = 1; i <= 8; i++) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) 
                  << (int)led_buffer[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "7-Seg Left (9-16):   ";
    for (int i = 9; i <= 16; i++) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) 
                  << (int)led_buffer[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Special (17-24):     ";
    for (int i = 17; i <= 24; i++) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) 
                  << (int)led_buffer[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Matrix (25-72):      ";
    for (int i = 25; i <= 72; i++) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) 
                  << (int)led_buffer[i] << " ";
        if ((i - 24) % 12 == 0) std::cout << std::endl << "                     ";
    }
    std::cout << std::endl;
    
    std::cout << "Stop (73-80):        ";
    for (int i = 73; i <= 80; i++) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) 
                  << (int)led_buffer[i] << " ";
    }
    std::cout << std::dec << std::endl;  // Return to decimal mode
}

/*
* Tests all LED types by briefly lighting them up
* Useful for verifying that all LEDs are working
* 
* @param device: Pointer to the opened HID device
*/
void testAllLEDs() {
    std::cout << "Testing all LEDs..." << std::endl;
    
    // Test matrix LEDs - cycle through colors
    std::cout << "Testing matrix LEDs with different colors..." << std::endl;
    LEDColor test_colors[] = {LEDColor::red, LEDColor::green, LEDColor::blue, LEDColor::white};
    
    for (int i = 0; i < 4; i++) {
        for (int row = 1; row <= 4; row++) {
            for (int col = 1; col <= 4; col++) {
                setMatrixButtonLED(row, col, test_colors[i], 0.5f);
                usleep(100000);  // Sleep for 100ms
            }
        }
        std::cout << "Matrix LEDs set to color " << i + 1 << "/4" << std::endl;
        // In a real application, you'd add a delay here
        usleep(500000);  // Sleep for 500ms to see the color
    }
    
    // Test special button LEDs
    std::cout << "Testing special button LEDs..." << std::endl;
    SpecialLEDButton special_buttons[] = {
        SpecialLEDButton::BROWSE, SpecialLEDButton::SIZE, SpecialLEDButton::TYPE,
        SpecialLEDButton::REVERSE, SpecialLEDButton::SHIFT, SpecialLEDButton::CAPTURE,
        SpecialLEDButton::QUANT, SpecialLEDButton::SYNC
    };
    
    for (int i = 0; i < 8; i++) {
        setSpecialButtonLED(special_buttons[i], 0.8f);
        usleep(100000);  // Sleep for 100ms
    }
    std::cout << "All special button LEDs turned on" << std::endl;
    
    // Test stop button LEDs
    std::cout << "Testing stop button LEDs..." << std::endl;
    for (int i = 1; i <= 4; i++) {
        setStopButtonLED(static_cast<StopLEDButton>(i), 0.8f);
        usleep(100000);  // Sleep for 100ms
    }
    std::cout << "All stop button LEDs turned on" << std::endl;

    // Sleep to see all colors
    usleep(1000000);  // Sleep for 1 second

    // Clear all LEDs at the end
    std::cout << "Test complete - clearing all LEDs" << std::endl;
    clearAllLEDs();
}