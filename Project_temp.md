# Button Toggle System - Complete Implementation Plan

## Overview

Implement a button toggle system that allows special buttons and matrix buttons to toggle between their original color/brightness (from current scene) and 100% brightness. This creates a visual feedback system for button states while preserving scene colors and patterns through parallel state storage.

## Core Concept

### Toggle Behavior

- **First press:** Button LED brightness → 1.0 (100%, preserving original color for matrix buttons)
- **Second press:** Button LED → original color and brightness values
- **Scene change:** Parallel storage automatically maintains original states

## Technical Solution: Parallel State Storage

### Problem Identified
- LED buffer stores final 7-bit values (0-127) after color/brightness conversion
- Original color and brightness information is lost due to mathematical conversion
- Cannot reverse-calculate original values due to rounding errors

### Solution: Extend LED Controller
Add parallel storage arrays to `led_controller.cpp` that preserve original color/brightness values alongside the existing LED buffer system.

## Implementation Plan

### Phase 1: Extend LED Controller with State Storage

#### Modify `headers/led_controller.h`

**Add new data structure:**
```cpp
// Add after existing structs
struct LEDState {
    LEDColor color;
    float brightness;
};

// Add new function declarations  
int getSpecialButtonIndex(SpecialLEDButton button);
LEDState getMatrixButtonState(int row, int col);
LEDState getSpecialButtonState(SpecialLEDButton button);
```

#### Modify `functions/led_controller.cpp`

**Add state storage arrays:**
```cpp
// Add after led_buffer declaration
static LEDState matrix_states[4][4];
static LEDState special_states[8];
```

**Add explicit enum mapping function:**
```cpp
int getSpecialButtonIndex(SpecialLEDButton button) {
    switch (button) {
        case SpecialLEDButton::BROWSE:  return 0;
        case SpecialLEDButton::SIZE:    return 1;
        case SpecialLEDButton::TYPE:    return 2;
        case SpecialLEDButton::REVERSE: return 3;
        case SpecialLEDButton::SHIFT:   return 4;
        case SpecialLEDButton::CAPTURE: return 5;
        case SpecialLEDButton::QUANT:   return 6;
        case SpecialLEDButton::SYNC:    return 7;
    }
    return -1; // Error case
}
```

**Modify existing setMatrixButtonLED():**
```cpp
bool setMatrixButtonLED(int row, int col, LEDColor color, float brightness) {
    // ... existing validation code ...
    
    // NEW: Save the original color and brightness
    matrix_states[row-1][col-1] = {color, brightness};
    
    // ... rest of existing function unchanged ...
}
```

**Modify existing setSpecialButtonLED():**
```cpp
bool setSpecialButtonLED(SpecialLEDButton button, float brightness) {
    // ... existing validation code ...
    
    // NEW: Save the original brightness
    int index = getSpecialButtonIndex(button);
    if (index >= 0 && index < 8) {
        special_states[index] = {LEDColor::white, brightness}; // Color irrelevant for special buttons
    }
    
    // ... rest of existing function unchanged ...
}
```

**Add getter functions:**
```cpp
LEDState getMatrixButtonState(int row, int col) {
    if (!isValidMatrixPosition(row, col)) {
        return {LEDColor::black, 0.0f}; // Error case
    }
    return matrix_states[row-1][col-1];
}

LEDState getSpecialButtonState(SpecialLEDButton button) {
    int index = getSpecialButtonIndex(button);
    if (index < 0 || index > 7) {
        return {LEDColor::white, 0.0f}; // Error case
    }
    return special_states[index];
}
```

#### Testing Phase 1
1. Set various LED colors/brightness using existing functions
2. Read back states using new getter functions  
3. Verify state storage accuracy
4. Confirm existing LED functionality remains unchanged

### Phase 2: Create Button Toggle System

#### Create `headers/btn_toggle_system.h`

```cpp
#ifndef BTN_TOGGLE_SYSTEM_H
#define BTN_TOGGLE_SYSTEM_H

#include "led_controller.h"

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

#endif // BTN_TOGGLE_SYSTEM_H
```

#### Create `functions/btn_toggle_system.cpp`

```cpp
#include "headers/btn_toggle_system.h"
#include <iostream>

bool ButtonToggleSystem::initialize() {
    std::cout << "Initializing Button Toggle System..." << std::endl;
    resetAllToggleStates();
    return true;
}

bool ButtonToggleSystem::toggleSpecialButton(SpecialLEDButton button) {
    int index = getSpecialButtonIndex(button);
    if (index < 0) {
        std::cerr << "Error: Invalid special button in toggle" << std::endl;
        return false;
    }
    
    if (special_toggled[index]) {
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
    if (!isValidMatrixPosition(row, col)) {
        std::cerr << "Error: Invalid matrix position in toggle" << std::endl;
        return false;
    }
    
    if (matrix_toggled[row-1][col-1]) {
        // Currently toggled → return to original color and brightness
        LEDState original = getMatrixButtonState(row, col);
        setMatrixButtonLED(row, col, original.color, original.brightness);
        matrix_toggled[row-1][col-1] = false;
        std::cout << "Matrix button (" << row << "," << col << ") toggled OFF" << std::endl;
    } else {
        // Currently original → toggle to full brightness (keep original color)
        LEDState original = getMatrixButtonState(row, col);
        setMatrixButtonLED(row, col, original.color, TOGGLED_BRIGHTNESS);
        matrix_toggled[row-1][col-1] = true;
        std::cout << "Matrix button (" << row << "," << col << ") toggled ON" << std::endl;
    }
    return true;
}

void ButtonToggleSystem::resetAllToggleStates() {
    // Reset special button toggle states
    for (int i = 0; i < 8; i++) {
        special_toggled[i] = false;
    }
    
    // Reset matrix button toggle states
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            matrix_toggled[row][col] = false;
        }
    }
    
    std::cout << "All toggle states reset to false" << std::endl;
}

void ButtonToggleSystem::printToggleStates() {
    std::cout << "=== Toggle States ===" << std::endl;
    
    std::cout << "Special buttons: ";
    for (int i = 0; i < 8; i++) {
        std::cout << (special_toggled[i] ? "1" : "0") << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Matrix buttons:" << std::endl;
    for (int row = 0; row < 4; row++) {
        std::cout << "  Row " << (row + 1) << ": ";
        for (int col = 0; col < 4; col++) {
            std::cout << (matrix_toggled[row][col] ? "1" : "0") << " ";
        }
        std::cout << std::endl;
    }
}
```

#### Testing Phase 2
1. Initialize toggle system after startup sequence
2. Test special button toggles - verify brightness changes correctly
3. Test matrix button toggles - verify color preservation and brightness changes
4. Test toggle state tracking with debug prints
5. Verify original states are properly restored

### Phase 3: Main Loop Integration

#### Program Startup Flow

```cpp
ButtonToggleSystem btn_toggle_system;

// 1. Initialize HIDAPI and device
// ... existing initialization code ...

// 2. Initialize LED controller
initializeLEDController(device);

// 3. Run startup sequence (sets initial LED colors/brightness)
startupSequence(device);

// 4. Initialize toggle system (resets all toggle states to false)
btn_toggle_system.initialize();

// 5. Enter main loop
```

#### Main Loop Integration

```cpp
// In main loop - detect button presses and toggle
unsigned char input_report_buffer[INPUT_REPORT_SIZE];

while (true) {
    if (readInputReport(device, input_report_buffer)) {
        
        // Toggle special buttons
        if (isSpecialButtonPressed(input_report_buffer, SpecialButton::SHIFT)) {
            btn_toggle_system.toggleSpecialButton(SpecialLEDButton::SHIFT);
        }
        
        if (isSpecialButtonPressed(input_report_buffer, SpecialButton::BROWSE)) {
            btn_toggle_system.toggleSpecialButton(SpecialLEDButton::BROWSE);
        }
        
        // Add remaining special buttons...
        
        // Toggle matrix buttons - example for position (1,1)
        if (isMatrixButtonPressed(input_report_buffer, 1, 1)) {
            btn_toggle_system.toggleMatrixButton(1, 1);
        }
        
        // Add remaining matrix button positions as needed...
    }
    
    // Small delay to prevent excessive CPU usage
    usleep(10000); // 10ms delay
}
```

#### Update CMakeLists.txt

```cmake
# Add new source file
add_executable(traktor 
    main.cpp
    functions/startup_sequence.cpp
    functions/led_controller.cpp
    functions/input_reader.cpp
    functions/btn_toggle_system.cpp
)
```

### Phase 4: Future Extensions

This design supports easy extension to:

#### Stop Button Toggles
```cpp
// Add to ButtonToggleSystem struct:
bool stop_toggled[4];

// Add function:
bool toggleStopButton(StopLEDButton button);
```

#### Scene System Integration
```cpp
// When implementing scene switching (future):
// 1. Set new LED patterns using existing functions
setMatrixButtonLED(1, 1, LEDColor::red, 0.5f);
setSpecialButtonLED(SpecialLEDButton::SHIFT, 0.3f);

// 2. Reset toggle states (new original states are automatically saved)
btn_toggle_system.resetAllToggleStates();
```

## Key Benefits

1. **Perfect State Preservation:** Parallel storage maintains original color and brightness values
2. **Backward Compatibility:** Existing LED controller API unchanged
3. **Extensible Design:** Easy to add more button types and features
4. **Scene-Ready:** Automatic state tracking supports future scene switching
5. **Clear Visual Feedback:** Toggle between original and full brightness states
6. **Maintainable Code:** Clear separation of concerns between modules

## File Structure After Implementation

```
project/
├── main.cpp
├── Project_State_1.md               ← Previous project summary  
├── Project_State_2_Updated.md       ← This implementation plan
├── functions/
│   ├── startup_sequence.cpp         ← Complete
│   ├── led_controller.cpp           ← MODIFIED: Added state storage
│   ├── input_reader.cpp             ← Complete
│   └── btn_toggle_system.cpp        ← NEW: Toggle functionality
├── headers/
│   ├── startup_sequence.h           ← Complete
│   ├── led_controller.h             ← MODIFIED: Added state functions
│   ├── input_reader.h               ← Complete
│   └── btn_toggle_system.h          ← NEW: Toggle system interface
└── build/
    └── traktor                      ← Updated executable
```

This implementation provides a robust foundation for button toggling while maintaining the established code quality and teaching approach of the project.