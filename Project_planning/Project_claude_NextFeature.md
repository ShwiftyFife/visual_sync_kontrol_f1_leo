# Button Toggle System - Implementation Plan

## Overview

Implement a button toggle system that allows special buttons and matrix buttons to toggle between their original color/brightness (from current scene) and 100% brightness. This creates a visual feedback system for button states while preserving scene colors and patterns through parallel state storage (in led_controller).

## Core Concept

### Toggle Behavior

- **First press:** Button LED brightness → 1.0 (100%, preserving original color for matrix buttons)
- **Second press:** Button LED → original color and brightness values
- **Scene change:** Parallel storage automatically maintains original states

## Implementation Plan

### Phase 1: Extend LED Controller with State Storage -- COMPLETED

#### Add new data structure and fuctions

```cpp
// Add after existing structs
struct LEDStateMatrix {
    LEDColor color;
    float brightness;
};

struct LEDState {
    float brightness;
};

// Add new function declarations  
int getSpecialButtonIndex(SpecialLEDButton button);
LEDStateMatrix getMatrixButtonState(int row, int col);
LEDState getSpecialButtonState(SpecialLEDButton button);
```

#### Store in arrays in cpp

```cpp
// Add after led_buffer declaration
static LEDState matrix_states[4][4];
static LEDState special_states[8];
```

#### Save state in setButtonLED functions

```cpp
bool setMatrixButtonLED(int row, int col, LEDColor color, float brightness) {
    // ... existing validation code ...
    
    // NEW: Save the original color and brightness
    matrix_states[row-1][col-1] = {color, brightness};
    
    // ... rest of existing function unchanged ...
}
```

### Phase 2: Create Button Toggle System

#### Create `headers/led_controller_toggle.h`

```cpp
#ifndef LED_CONTROLLER_TOGGLE_H
#define LED_CONTROLLER_TOGGLE_H

#include "led_controller_toggler.h"

struct ButtonToggleSystem {
    // Toggle state tracking arrays
    bool special_toggled[8];           // Track which special buttons are toggled
    bool matrix_toggled[4][4];         // Track which matrix buttons are toggled
    
    // Constants
    static constexpr float TOGGLED_BRIGHTNESS = 1.0f;
    
    // Core functions
    bool initialize();
    bool toggleSpecialButton(SpecialLEDButton button);
    bool toggleMatrixButton(int row, int col);
    void resetAllToggleStates();
    
    // Utility functions
    void printToggleStates();
};

#endif // LED_CONTROLLER_TOGGLE_H
```

#### Create `functions/led_controller_toggle.cpp`

```cpp
#include "headers/led_controller_toggle.h"
#include <iostream>

bool ButtonToggleSystem::initialize() {
    std::cout << "Initializing Button Toggle System..." << std::endl;
    resetAllToggleStates();
    return true;
}

bool ButtonToggleSystem::toggleSpecialButton(SpecialLEDButton button) {
    int index = getSpecialButtonIndex(button);
    // if statement as validation for index >= 0
    
    if (special_toggled[index] == true) {
        // Currently toggled → return to original brightness
        LEDState original = getSpecialButtonState(button);
        setSpecialButtonLED(button, original.brightness);
        special_toggled[index] = false;
        std::cout << "Special button " << index << " toggled OFF" << std::endl;
    } else {
        // Currently original → toggle to full brightness
        setSpecialButtonLED(button, TOGGLED_BRIGHTNESS);
        special_toggled[index] = true;
        std::cout << "Special button " << index << " toggled ON" << std::endl;
    }
    return true;
}

bool ButtonToggleSystem::toggleMatrixButton(int row, int col) {
    // ...
    return true;
}

void ButtonToggleSystem::resetAllToggleStates() {
    // Reset special button toggle states...
    // Reset matrix button toggle states...   
}

void ButtonToggleSystem::printToggleStates() {
    //...
}
```

#### Testing Phase 2

1. Initialize toggle system after startup sequence
2. Test special button toggles - verify brightness changes correctly
3. Test matrix button toggles - verify color preservation and brightness changes
4. Test toggle state tracking with debug prints
5. Verify original states are properly restored

### Phase 3: Main Loop Integration

```cpp
ButtonToggleSystem btn_toggle_system;

// ... existing initialization code ...

initializeLEDController(device);   // Initialize LED controller
startupSequence(device);           // Run startup sequence

// 2. Initialize toggle system (resets all toggle states to false)
btn_toggle_system.initialize();

// 5. Enter main loop
```
