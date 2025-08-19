#include "headers/set_matrix_led.h"

#include <stdio.h>
#include <cstring>

// Constructor
MatrixLEDController::MatrixLEDController(hid_device *handle) : device_handle(handle) {
    // Initialize buffer with report ID and all LEDs off
    memset(led_buffer, 0, sizeof(led_buffer));
    led_buffer[0] = 0x80; // Set report ID
}

// Get the byte positions for a specific matrix button
ButtonMapping MatrixLEDController::getButtonMapping(MatrixButton button) {
    ButtonMapping mapping;
    
    switch(button) {
        case MatrixButton::matrix11: mapping = {25, 26, 27}; break;
        case MatrixButton::matrix12: mapping = {28, 29, 30}; break;
        case MatrixButton::matrix13: mapping = {31, 32, 33}; break;
        case MatrixButton::matrix14: mapping = {34, 35, 36}; break;
        case MatrixButton::matrix21: mapping = {37, 38, 39}; break;
        case MatrixButton::matrix22: mapping = {40, 41, 42}; break;
        case MatrixButton::matrix23: mapping = {43, 44, 45}; break;
        case MatrixButton::matrix24: mapping = {46, 47, 48}; break;
        case MatrixButton::matrix31: mapping = {49, 50, 51}; break;
        case MatrixButton::matrix32: mapping = {52, 53, 54}; break;
        case MatrixButton::matrix33: mapping = {55, 56, 57}; break;
        case MatrixButton::matrix34: mapping = {58, 59, 60}; break;
        case MatrixButton::matrix41: mapping = {61, 62, 63}; break;
        case MatrixButton::matrix42: mapping = {64, 65, 66}; break;
        case MatrixButton::matrix43: mapping = {67, 68, 69}; break;
        case MatrixButton::matrix44: mapping = {70, 71, 72}; break;
    }
    
    return mapping;
}

// Send current buffer state to the device
bool MatrixLEDController::sendToDevice() {
    if (!device_handle) {
        printf("Error: Invalid device handle\n");
        return false;
    }
    
    int res = hid_write(device_handle, led_buffer, sizeof(led_buffer));
    if (res < 0) {
        printf("Error writing to device: %ls\n", hid_error(device_handle));
        return false;
    }
    
    return true;
}

// Main function: Set a matrix button to a specific color and brightness
bool MatrixLEDController::setMatrixButtonLED(MatrixButton button, ColorSystem color, float brightness) {
    // Get the color values with brightness applied
    BRGColor colorValues = getColorWithBrightness(color, brightness);
    
    // Get the byte positions for this button
    ButtonMapping mapping = getButtonMapping(button);
    
    // Update the buffer (this preserves all other LED states!)
    led_buffer[mapping.blue_pos] = colorValues.blue;
    led_buffer[mapping.red_pos] = colorValues.red;
    led_buffer[mapping.green_pos] = colorValues.green;
    
    // Send to device
    return sendToDevice();
}

// Turn off all 16 matrix buttons
bool MatrixLEDController::clearAllMatrixLEDs() {
    // Clear all matrix button positions (bytes 25-72)
    for (int i = 25; i <= 72; i++) {
        led_buffer[i] = 0;
    }
    
    return sendToDevice();
}

// Turn off one specific button
bool MatrixLEDController::clearMatrixButton(MatrixButton button) {
    ButtonMapping mapping = getButtonMapping(button);
    
    led_buffer[mapping.blue_pos] = 0;
    led_buffer[mapping.red_pos] = 0;
    led_buffer[mapping.green_pos] = 0;
    
    return sendToDevice();
}

// Send current state to device (useful if you want to update without changing anything)
bool MatrixLEDController::updateDevice() {
    return sendToDevice();
}

// For testing/debugging - print the byte positions for a button
void MatrixLEDController::printButtonMapping(MatrixButton button) {
    ButtonMapping mapping = getButtonMapping(button);
    
    const char* buttonNames[] = {
        "matrix11", "matrix21", "matrix31", "matrix41",
        "matrix12", "matrix22", "matrix32", "matrix42", 
        "matrix13", "matrix23", "matrix33", "matrix43",
        "matrix14", "matrix24", "matrix34", "matrix44"
    };
    
    printf("%s: Blue=%d, Red=%d, Green=%d\n", 
           buttonNames[(int)button], 
           mapping.blue_pos, mapping.red_pos, mapping.green_pos);
}