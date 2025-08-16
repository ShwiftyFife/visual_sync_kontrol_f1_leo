#ifndef MATRIX_LED_H
#define MATRIX_LED_H

#include <hidapi/hidapi.h>
#include "color_system.h"

// Enum for all 16 matrix buttons
enum class MatrixButton {
    matrix11, matrix21, matrix31, matrix41,
    matrix12, matrix22, matrix32, matrix42, 
    matrix13, matrix23, matrix33, matrix43,
    matrix14, matrix24, matrix34, matrix44
};

// Structure to hold the byte positions for one matrix button
struct ButtonMapping {
    int blue_pos;
    int red_pos; 
    int green_pos;
};

// Main LED controller class for matrix buttons
class MatrixLEDController {
private:
    hid_device *device_handle;
    unsigned char led_buffer[81];        // The main buffer we send to device
    
    // Get the byte positions for a specific matrix button
    ButtonMapping getButtonMapping(MatrixButton button);
    
    // Send current buffer state to the device
    bool sendToDevice();

public:
    // Constructor
    MatrixLEDController(hid_device *handle);
    
    // Main function: Set a matrix button to a specific color and brightness
    bool setMatrixButtonLED(MatrixButton button, ColorSystem color, float brightness);
    
    // Utility functions
    bool clearAllMatrixLEDs();           // Turn off all 16 matrix buttons
    bool clearMatrixButton(MatrixButton button);  // Turn off one specific button
    bool updateDevice();                 // Send current state to device
    
    // For testing/debugging
    void printButtonMapping(MatrixButton button);
};

#endif // MATRIX_LED_H