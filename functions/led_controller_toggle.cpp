#include "headers/led_controller_toggle.h"  // Include header file
#include "headers/input_reader.h"           // Include input reader header
#include "headers/led_controller.h"         // Access to LED controller functions and state structures

#include <iostream>  // For console output

// =============================================================================
// Update button states based on input report
// =============================================================================
void ButtonToggleSystem::updateButtonStates(unsigned char* input_report) {
    // Update special button states
    special_was_pressed[0] = isSpecialButtonPressed(input_report, SpecialButton::BROWSE);
    special_was_pressed[1] = isSpecialButtonPressed(input_report, SpecialButton::SIZE);
    special_was_pressed[2] = isSpecialButtonPressed(input_report, SpecialButton::TYPE);
    special_was_pressed[3] = isSpecialButtonPressed(input_report, SpecialButton::REVERSE);
    special_was_pressed[4] = isSpecialButtonPressed(input_report, SpecialButton::SHIFT);
    special_was_pressed[5] = isControlButtonPressed(input_report, ControlButton::CAPTURE);
    special_was_pressed[6] = isControlButtonPressed(input_report, ControlButton::QUANT);
    special_was_pressed[7] = isControlButtonPressed(input_report, ControlButton::SYNC);
}

// =============================================================================
// BUTTON TOGGLE SYSTEM - Toggle buttons between original and full brightness
// =============================================================================

/*
* Initializes the button toggle system
* Resets all toggle states to false (all buttons in original state)
* 
* @return: true if initialization successful
*/
bool ButtonToggleSystem::initialize() {
    std::cout << "Initializing Button Toggle System..." << std::endl;
    
    // Reset all led toggle states to false
    resetAllToggleStates();

    // initialize button state tracking to false
    for (int i = 0; i < 8; i++) {
        special_was_pressed[i] = false;
    }

    // Output initialization status
    std::cout << "  - All toggle states reset to original" << std::endl;
    return true;
}

/*
* Resets all toggle states to false (original state)
* This means all buttons return to their scene colors/brightness
*/
void ButtonToggleSystem::resetAllToggleStates() {
    // Reset special button toggle states
    for (int i = 0; i < 8; i++) {
        special_toggled[i] = false;
    }
}

// =============================================================================
// Toggle button functions
// =============================================================================

/*
* Toggles a special button between original brightness and full brightness
* Preserves the original brightness value using the LED controller's state storage
* 
* @param button: Which special button to toggle
* @return: true if successful, false if error
*/
bool ButtonToggleSystem::toggleSpecialButton(SpecialLEDButton button) {
    // Step 1: Get array index for this button (validate it exists)
    int index = getSpecialButtonIndex(button);
    if (index < 0) {
        std::cerr << "Error: Invalid special button in toggleSpecialButton()" << std::endl;
        return false;
    }

    // Step 2: Check current toggle state and act accordingly

    // Currently original → toggle to full brightness
    if (special_toggled[index] == false) {
        // Set to full brightness, DONT overwrite original state!
        setSpecialButtonLED(button, TOGGLED_BRIGHTNESS, false);
        // Update toggle state
        special_toggled[index] = true;
    } else if (special_toggled[index] == true) { // Currently toggled → return to original brightness
        // Get original state
        LEDState original = getSpecialButtonState(button);
        // Set to original brightness, DONT overwrite original state!
        setSpecialButtonLED(button, original.brightness, false);
        // Update toggle state
        special_toggled[index] = false;
    }
    
    return true;
}

/*
* Check if button should trigger toggle - only on press (not held)
* Call this instead of calling toggleSpecialButton directly
*
* @return: true if toggle was triggered, false otherwise
*/
bool ButtonToggleSystem::shouldToggleSpecialButton(unsigned char* input_data, SpecialButton button, SpecialLEDButton led_button) {
    // Step 1: Get array index for this button (validate it exists)
    int index = getSpecialButtonIndex(led_button);
    if (index < 0) {
        std::cerr << "Error: Invalid special button in shouldToggleSpecialButton()" << std::endl;
        return false;
    }

    // Step 2: Check current and previous button states
    bool currently_pressed = isSpecialButtonPressed(input_data, button);
    bool was_pressed = special_was_pressed[index];
    
    // Step 3: Only trigger on press transition (false -> true)
    // Call toggle function if pressed and was not pressed
    if (currently_pressed && !was_pressed) {
        return toggleSpecialButton(led_button);
    }
    
    return false;
}