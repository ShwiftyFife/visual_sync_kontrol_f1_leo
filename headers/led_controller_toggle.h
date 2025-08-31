#ifndef LED_CONTROLLER_TOGGLE_H
#define LED_CONTROLLER_TOGGLE_H

#include "input_reader.h"   // Access to the input reading functions
#include "led_controller.h" // Access to the LED controller functions and state structures

struct ButtonToggleSystem {
    // Toggle state tracking arrays
    bool special_toggled[8];           // Track which special buttons are toggled
    bool stop_toggled[4];              // Track which stop buttons are toggled
    bool matrix_toggled[4][4];         // Track which matrix buttons are toggled
    
    // Toggle state tracking for immediate button toggle - track if button was tracked in last while(true) frame
    bool special_was_pressed[8];       // Track if special button was pressed in last frame
    bool stop_was_pressed[4];          // Track if stop button was pressed in last frame
    bool matrix_was_pressed[4][4];     // Track if matrix button was pressed in last frame

    // Constants
    static constexpr float TOGGLED_BRIGHTNESS = 1.0f;
    
    // Core functions
    bool initialize();
    bool toggleSpecialButton(SpecialLEDButton button);
    bool shouldToggleSpecialButton(unsigned char* input_data, SpecialButton button, SpecialLEDButton led_button);
    void updateButtonStates(unsigned char* input_report);
    void resetAllToggleStates();
    
    // Utility functions
    //void printToggleStates();
};

#endif // LED_CONTROLLER_TOGGLE_H