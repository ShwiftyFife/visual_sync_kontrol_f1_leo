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

    // Update control button states
    control_was_pressed[0] = isControlButtonPressed(input_report, ControlButton::CAPTURE);
    control_was_pressed[1] = isControlButtonPressed(input_report, ControlButton::QUANT);
    control_was_pressed[2] = isControlButtonPressed(input_report, ControlButton::SYNC);

    // Update stop button states
    stop_was_pressed[0] = isStopButtonPressed(input_report, StopButton::STOP1);
    stop_was_pressed[1] = isStopButtonPressed(input_report, StopButton::STOP2);
    stop_was_pressed[2] = isStopButtonPressed(input_report, StopButton::STOP3);
    stop_was_pressed[3] = isStopButtonPressed(input_report, StopButton::STOP4);

    // Update matrix button states
    for (int row = 1; row <= 4; row++) {
        for (int col = 1; col <= 4; col++) {
            matrix_was_pressed[row][col] = isMatrixButtonPressed(input_report, row, col);
        }
    }
}

// =============================================================================
// BUTTON TOGGLE SYSTEM - Toggle buttons between original and full brightness
// =============================================================================

/*
* Initializes the button toggle system
* Resets all toggle states to false (all buttons in original state)
* Sets default matrix toggle mode to FULL_MATRIX
* 
* @return: true if initialization successful
*/
bool ButtonToggleSystem::initialize() {
    std::cout << "Initializing Button Toggle System..." << std::endl;

    // Set default matrix toggle mode
    current_matrix_mode = MatrixToggleZone::FULL_MATRIX;

    // Reset all led toggle states to false
    resetAllToggleStates();    

    // Output initialization status
    std::cout << "  - All toggle states reset to original" << std::endl;
    std::cout << "  - Matrix toggle mode set to FULL_MATRIX" << std::endl;
    return true;
}

/*
* Resets all toggle states to false (original state)
* This means all buttons return to their scene colors/brightness
*/
void ButtonToggleSystem::resetAllToggleStates() {
    // ________________________________________
    // Reset button press tracking states
        // SPECIAL BUTTONS
        for (int i = 0; i < 5; i++) {
            special_was_pressed[i] = false;
        }
        // CONTROL BUTTONS
        for (int i = 0; i < 3; i++) {
            control_was_pressed[i] = false;
        }
        // STOP BUTTONS
        for (int i = 0; i < 4; i++) {
            stop_was_pressed[i] = false;
        }
        // MATRIX BUTTONS
        for (int row = 1; row <= 4; row++) {
            for (int col = 1; col <= 4; col++) {
                matrix_was_pressed[row][col] = false;
            }
        }

    // ________________________________________
    // Reset all toggle states to false (original state)
        
        // SPECIAL BUTTONS
        for (int i = 0; i < 5; i++) {
            special_toggled[i] = false;
        }

        // CONTROL BUTTONS
        for (int i = 0; i < 3; i++) {
            control_toggled[i] = false;
        }

        // STOP BUTTONS
        for (int i = 0; i < 4; i++) {
            stop_toggled[i] = false;
        }
        // MATRIX BUTTONS
        for (int row = 1; row <= 4; row++) {
            for (int col = 1; col <= 4; col++) {
                matrix_toggled[row][col] = false;
            }
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
* Toggles a control button between original brightness and full brightness
* Preserves the original brightness value using the LED controller's state storage
*
* @param button: Which control button to toggle
* @return: true if successful, false if error
*/
bool ButtonToggleSystem::toggleControlButton(ControlLEDButton button) {
    // Step 1: Get array index for this button (validate it exists)
    int index = getControlButtonIndex(button);
    if (index < 0) {
        std::cerr << "Error: Invalid control button in toggleControlButton()" << std::endl;
        return false;
    }

    // Step 2: Check current toggle state and act accordingly

    // Currently original → toggle to full brightness
    if (control_toggled[index] == false) {
        // Set to full brightness, DONT overwrite original state!
        setControlButtonLED(button, TOGGLED_BRIGHTNESS, false);
        // Update toggle state
        control_toggled[index] = true;
    } else if (control_toggled[index] == true) { // Currently toggled → return to original brightness
        // Get original state
        LEDState original = getControlButtonState(button);
        // Set to original brightness, DONT overwrite original state!
        setControlButtonLED(button, original.brightness, false);
        // Update toggle state
        control_toggled[index] = false;
    }

    return true;
}

/*
* Toggles a stop button between original brightness and full brightness
* Preserves the original brightness value using the LED controller's state storage
*
* @param button: Which stop button to toggle
* @return: true if successful, false if error
*/
bool ButtonToggleSystem::toggleStopButton(StopLEDButton button) {
    // Step 1: Get array index for this button (validate it exists)
    int index = getStopButtonIndex(button);
    if (index < 0) {
        std::cerr << "Error: Invalid stop button in toggleStopButton()" << std::endl;
        return false;
    }

    // Step 2: Check current toggle state and act accordingly

    // Currently original → toggle to full brightness
    if (stop_toggled[index] == false) {
        // Set to full brightness, DONT overwrite original state!
        setStopButtonLED(button, TOGGLED_BRIGHTNESS, false);
        // Update toggle state
        stop_toggled[index] = true;
    } else if (stop_toggled[index] == true) { // Currently toggled → return to original brightness
        // Get original state
        LEDState original = getStopButtonState(button);
        // Set to original brightness, DONT overwrite original state!
        setStopButtonLED(button, original.brightness, false);
        // Update toggle state
        stop_toggled[index] = false;
    }

    return true;
}

/*
* Toggles a matrix button between original brightness and full brightness
* Preserves the original brightness value using the LED controller's state storage
* NOTE: This function only handles the actual toggle - zone management is done in shouldToggleMatrixButton()
*
* @param row: The row of the matrix button (1-4)
* @param col: The column of the matrix button (1-4)
* @return: true if successful, false if error
*/
bool ButtonToggleSystem::toggleMatrixButton(int row, int col) {
    // Step 1: Validate row and column ranges
    if (row < 1 || row > 4 || col < 1 || col > 4) {
        std::cerr << "Error: Invalid matrix position (" << row << "," << col 
                  << "). Must be 1-4 for both row and column." << std::endl;
        return false;
    }

    // Step 2: Check current toggle state and act accordingly

    // Currently original → toggle to full brightness
    if (matrix_toggled[row][col] == false) {
        // Get original state
        LEDStateMatrix original = getMatrixButtonState(row, col);
        // Set to full brightness, DONT overwrite original state!
        setMatrixButtonLED(row, col, original.color, TOGGLED_BRIGHTNESS, false);
        // Update toggle state
        matrix_toggled[row][col] = true;
    } else if (matrix_toggled[row][col] == true) { // Currently toggled → return to original brightness
        // Get original state
        LEDStateMatrix original = getMatrixButtonState(row, col);
        // Set to original brightness, DONT overwrite original state!
        setMatrixButtonLED(row, col, original.color, original.brightness, false);
        // Update toggle state
        matrix_toggled[row][col] = false;
    }

    return true;
}

// =============================================================================
// RUNTIME Button Toggle Checks
// =============================================================================

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

bool ButtonToggleSystem::shouldToggleControlButton(unsigned char* input_data, ControlButton button, ControlLEDButton led_button) {
    // Step 1: Get array index for this button (validate it exists)
    int index = getControlButtonIndex(led_button);
    if (index < 0) {
        std::cerr << "Error: Invalid control button in shouldToggleControlButton()" << std::endl;
        return false;
    }

    // Step 2: Check current and previous button states
    bool currently_pressed = isControlButtonPressed(input_data, button);
    bool was_pressed = control_was_pressed[index];
    
    // Step 3: Only trigger on press transition (false -> true)
    // Call toggle function if pressed and was not pressed
    if (currently_pressed && !was_pressed) {
        return toggleControlButton(led_button);
    }
    
    return false;
}

bool ButtonToggleSystem::shouldToggleStopButton(unsigned char* input_data, StopButton button, StopLEDButton led_button) {
    // Step 1: Get array index for this button (validate it exists)
    int index = getStopButtonIndex(led_button);
    if (index < 0) {
        std::cerr << "Error: Invalid stop button in shouldToggleStopButton()" << std::endl;
        return false;
    }

    // Step 2: Check current and previous button states
    bool currently_pressed = isStopButtonPressed(input_data, button);
    bool was_pressed = stop_was_pressed[index];
    
    // Step 3: Only trigger on press transition (false -> true)
    // Call toggle function if pressed and was not pressed
    if (currently_pressed && !was_pressed) {
        return toggleStopButton(led_button);
    }
    
    return false;
}

// =============================================================================
// ENHANCED MATRIX BUTTON TOGGLE WITH ZONE SYSTEM
// =============================================================================

/*
* Enhanced matrix button toggle with automatic zone-based untoggling
* This is the main function that implements the "untoggle" behavior
*
* @param input_data: Current input report
* @param row: Matrix button row (1-4)  
* @param col: Matrix button column (1-4)
* @return: true if toggle action was performed, false otherwise
*/
bool ButtonToggleSystem::shouldToggleMatrixButton(unsigned char* input_data, int row, int col) {
    // Step 1: Validate matrix position
    if (row < 1 || row > 4 || col < 1 || col > 4) {
        std::cerr << "Error: Invalid matrix position (" << row << "," << col << ") in shouldToggleMatrixButton()" << std::endl;
        return false;
    }

    // Step 2: Check current and previous button states
    bool currently_pressed = isMatrixButtonPressed(input_data, row, col);
    bool was_pressed = matrix_was_pressed[row][col];

    // Step 3: Only trigger on press transition (false -> true)
    if (currently_pressed && !was_pressed) {

        // Step 4: Check if this button is already toggled
        if (matrix_toggled[row][col] == true) {
            // Button is already toggled - just untoggle it directly
            return toggleMatrixButton(row, col);
        } else {
            // Button is not toggled - untoggle others in zone, then toggle this one
            
            // Step 5: Determine which zone this button belongs to
            int target_zone = getButtonZone(row, col);
            
            // Step 6: Auto-untoggle all other buttons in the same zone
            untoggleMatrixZone(target_zone);
            
            // Step 7: Toggle the pressed button
            return toggleMatrixButton(row, col);
        }
    }

    return false;
}

// =============================================================================
// NEW ZONE MANAGEMENT FUNCTIONS
// =============================================================================

/*
* Sets the matrix toggle mode (FULL_MATRIX or LEFT_RIGHT_SPLIT)
* 
* @param mode: The desired toggle zone mode
*/
void ButtonToggleSystem::setMatrixToggleMode(MatrixToggleZone mode) {
    current_matrix_mode = mode;
    
    // Clear all matrix toggles when changing modes to avoid confusion
    untoggleAllMatrixButtons();
}

/*
* Gets the current matrix toggle mode
* 
* @return: Current MatrixToggleZone mode
*/
MatrixToggleZone ButtonToggleSystem::getMatrixToggleMode() const {
    return current_matrix_mode;
}

/*
* Determines which zone a matrix button belongs to based on current mode
* 
* @param row: Matrix button row (1-4)
* @param col: Matrix button column (1-4)  
* @return: Zone ID (0 for full matrix, 0/1 for left/right split)
*/
int ButtonToggleSystem::getButtonZone(int row, int col) const {
    // Validate input
    if (row < 1 || row > 4 || col < 1 || col > 4) {
        std::cerr << "Error: Invalid matrix position in getButtonZone()" << std::endl;
        return -1; // Invalid zone
    }
    
    switch (current_matrix_mode) {
        case MatrixToggleZone::FULL_MATRIX:
            return 0; // All buttons in same zone
            
        case MatrixToggleZone::LEFT_RIGHT_SPLIT:
            if (col <= 2) {
                return 0; // Left zone (columns 1-2)
            } else {
                return 1; // Right zone (columns 3-4)  
            }
            
        default:
            std::cerr << "Error: Unknown matrix toggle mode" << std::endl;
            return -1;
    }
}

/*
* Untoggle all buttons in a specific zone
* 
* @param zone_id: Zone to clear (0 for left/full, 1 for right)
*/
void ButtonToggleSystem::untoggleMatrixZone(int zone_id) {
    // Iterate through all matrix buttons
    for (int row = 1; row <= 4; row++) {
        for (int col = 1; col <= 4; col++) {
            
            // Check if this button belongs to the target zone
            if (getButtonZone(row, col) == zone_id) {
                
                // If button is currently toggled, untoggle it
                if (matrix_toggled[row][col] == true) {
                    // Get original state and restore it
                    LEDStateMatrix original = getMatrixButtonState(row, col);
                    setMatrixButtonLED(row, col, original.color, original.brightness, false);
                    
                    // Update toggle state
                    matrix_toggled[row][col] = false;
                }
            }
        }
    }
}

/*
* Untoggle all matrix buttons (convenience function)
* Useful when switching modes or resetting
*/
void ButtonToggleSystem::untoggleAllMatrixButtons() {
    // Iterate through all matrix buttons
    for (int row = 1; row <= 4; row++) {
        for (int col = 1; col <= 4; col++) {
            
            // If button is currently toggled, untoggle it
            if (matrix_toggled[row][col] == true) {
                // Get original state and restore it
                LEDStateMatrix original = getMatrixButtonState(row, col);
                setMatrixButtonLED(row, col, original.color, original.brightness, false);
                
                // Update toggle state
                matrix_toggled[row][col] = false;
            }
        }
    }
}