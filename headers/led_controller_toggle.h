#ifndef LED_CONTROLLER_TOGGLE_H
#define LED_CONTROLLER_TOGGLE_H

#include "input_reader.h"   // Access to the input reading functions
#include "led_controller.h" // Access to the LED controller functions and state structures

// =============================================================================
// MATRIX ZONE SYSTEM - For exclusive button toggling
// =============================================================================

enum class MatrixToggleZone {
    FULL_MATRIX,        // All 16 buttons = 1 zone (default behavior)
    LEFT_RIGHT_SPLIT    // Columns 1-2 and 3-4 = 2 independent zones
};

// =============================================================================
// BUTTON TOGGLE SYSTEM - Main
// =============================================================================

struct ButtonToggleSystem {

    // =======================================
    // Variables to track button toggle states
    // =======================================
    // Toggle state tracking arrays
    bool special_toggled[5];           // Track which special buttons are toggled
    bool control_toggled[3];           // Track which control buttons are toggled
    bool stop_toggled[4];              // Track which stop buttons are toggled
    bool matrix_toggled[5][5];         // Track which matrix buttons are toggled
    
    // Toggle state tracking for immediate button toggle - track if button was tracked in last while(true) frame
    bool special_was_pressed[5];       // Track if special button was pressed in last frame
    bool control_was_pressed[3];       // Track if control button was pressed in last frame
    bool stop_was_pressed[4];          // Track if stop button was pressed in last frame
    bool matrix_was_pressed[5][5];     // Track if matrix button was pressed in last frame

    // =======================================
    // Matrix zone mode configuration
    // =======================================
    MatrixToggleZone current_matrix_mode;  // Current zone configuration

    // =======================================
    // Matrix toggle brightness constant
    // =======================================
    static constexpr float TOGGLED_BRIGHTNESS = 1.0f;
    
    // =======================================
    // Matrix main functions
    // =======================================

    // Core functions
    bool initialize();

    bool toggleSpecialButton(SpecialLEDButton button);
    bool toggleControlButton(ControlLEDButton button);
    bool toggleStopButton(StopLEDButton button);
    bool toggleMatrixButton(int row, int col);

    bool shouldToggleSpecialButton(unsigned char* input_data, SpecialButton button, SpecialLEDButton led_button);
    bool shouldToggleControlButton(unsigned char* input_data, ControlButton button, ControlLEDButton led_button);
    bool shouldToggleStopButton(unsigned char* input_data, StopButton button, StopLEDButton led_button);
    bool shouldToggleMatrixButton(unsigned char* input_data, int row, int col);

    void updateButtonStates(unsigned char* input_report);
    void resetAllToggleStates();
    
    // =======================================
    // Matrix zone toggle functions
    // =======================================
    // Zone configuration
    void setMatrixToggleMode(MatrixToggleZone mode);
    MatrixToggleZone getMatrixToggleMode() const;

     // Zone utility functions
    int getButtonZone(int row, int col) const;
    void untoggleMatrixZone(int zone_id);
    void untoggleAllMatrixButtons();
    
};

#endif // LED_CONTROLLER_TOGGLE_H