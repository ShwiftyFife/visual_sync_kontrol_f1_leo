# Traktor Kontrol F1 Toggle System - Continuation Summary

## Project Overview

C++ project using HIDAPI to control a Traktor Kontrol F1 device. Read HID input reports and control LEDs on the F1. Built with CMake, developed in VS Code on macOS/Ubuntu.

## Current Status: DEBUGGING TOGGLE SYSTEM

### What's Working

1. **Complete base systems:**

   - Input reader system (22-byte HID reports)
   - LED controller with state storage (81-byte LED reports)
   - Startup sequence with diagonal wave animation
   - Full HID communication (read input, send LED commands)

2. **Toggle system implementation - PARTIALLY COMPLETE:**

   - Header file: `headers/led_controller_toggle.h`
   - Implementation file: `functions/led_controller_toggle.cpp`
   - CMakeLists.txt updated
   - Modified LED controller functions with `save_state` parameter

### Current Problem: Toggle Function Not Working 🔍

**Symptoms:**

- Button flashes briefly when pressed
- Toggle function appears to be called multiple times rapidly
- Expected behavior: Single toggle per button press/release cycle

**Suspected Issue:**

- while(true) loop sends hid report constantly with no delay (which is importanf for faders)
- hence, when pressing button there is multiple reports with pressed button activating toggle function

## Code Structure

### File Organization

```{}
project/
├── main.cpp
├── functions/
│   ├── startup_sequence.cpp
│   ├── led_controller.cpp          
│   ├── input_reader.cpp            
│   └── led_controller_toggle.cpp   ← NEW!
├── headers/
│   ├── startup_sequence.h
│   ├── led_controller.h            
│   ├── input_reader.h              
│   └── led_controller_toggle.h     ← NEW!
└── build/
    └── traktor
```

### Key Data Structures

#### LED State Storage (in led_controller)

```cpp
struct LEDStateMatrix {
    LEDColor color;      // Original color
    float brightness;    // Original brightness
};

struct LEDState {
    float brightness;    // Original brightness
};

// Global state arrays
static LEDStateMatrix matrix_states[4][4];
static LEDState special_states[8];
static LEDState stop_states[4];
```

#### Toggle System Structure

```cpp
struct ButtonToggleSystem {
    bool special_toggled[8];           // Track which special buttons are toggled
    bool matrix_toggled[4][4];         // Track which matrix buttons are toggled
    static constexpr float TOGGLED_BRIGHTNESS = 1.0f;
    
    bool initialize();
    bool toggleSpecialButton(SpecialLEDButton button);
    bool toggleMatrixButton(int row, int col);       // NOT YET IMPLEMENTED
    void resetAllToggleStates();
    void printToggleStates();                        // NOT YET IMPLEMENTED
};
```

## Modified Functions

### LED Controller Changes

Added optional `save_state` parameter to preserve original states during toggling:

```cpp
// In led_controller.h
bool setSpecialButtonLED(SpecialLEDButton button, float brightness, bool save_state = true);
bool setMatrixButtonLED(int row, int col, LEDColor color, float brightness, bool save_state = true);

// In led_controller.cpp - modified to only save state when save_state == true
if (save_state) {
    special_states[index] = {brightness};
}
```

## Current Implementation

### Toggle System Functions (COMPLETED)

#### initialize()

- Calls `resetAllToggleStates()`
- Prints initialization message
- Returns true

#### resetAllToggleStates()

- Sets all `special_toggled[i] = false`
- Sets all `matrix_toggled[row][col] = false`

#### toggleSpecialButton()

```cpp
bool ButtonToggleSystem::toggleSpecialButton(SpecialLEDButton button) {
    int index = getSpecialButtonIndex(button);
    if (index < 0) return false;

    if (special_toggled[index] == true) {
        // Currently toggled → return to original brightness
        LEDState original = getSpecialButtonState(button);
        setSpecialButtonLED(button, original.brightness, false);  // save_state = false
        special_toggled[index] = false;
    } else {
        // Currently original → toggle to full brightness
        setSpecialButtonLED(button, TOGGLED_BRIGHTNESS, false);   // save_state = false
        special_toggled[index] = true;
    }
    return true;
}
```

### Main Loop Integration (CURRENT ISSUE)

Current main.cpp structure:

```cpp
int main() {
    // Declare toggle system early for scope
    ButtonToggleSystem btn_toggle_system;
    
    // ... device initialization ...
    
    // Initialize systems
    initializeLEDController(device);
    startupSequence(device);
    btn_toggle_system.initialize();
    
    // Button press detection variables
    bool prev_shift_pressed = false;
    
    while (true) {
        readInputReport(device, input_data);
        
        bool curr_shift_pressed = isSpecialButtonPressed(input_data, SpecialButton::SHIFT);
        
        // Only toggle on press (false → true transition)
        if (curr_shift_pressed && !prev_shift_pressed) {
            btn_toggle_system.toggleSpecialButton(SpecialLEDButton::SHIFT);
        }
        
        prev_shift_pressed = curr_shift_pressed;
    }
}
```

## Toggle function suggested solution

Struct saving the state of the button (good name missing):

```cpp
// Base structure
struct CurrentButtonState {
  bool pressed;
  bool toggled;
  bool released;
}

// Steps for button toggle:

// Step 1:
// All CurrentButtonState statements fhould be false
pressed = false;
toggled = false;
released = false;


// Step 2: 
// When button is being pressed: 
if (isSpecialButtonPressed(input_data, SpecialButton::SHIFT) == true) {

  // Step 2.1: pressed should be set to true:
  pressed = true;

  // Step 2.2: Call toggle function when button is pressed (pressed = true) and set toggled to true
  // This should avoid the toggle system to be called again as the toggle bool is set to true while the function is only called when its set to false
  if (pressed == true && released == false && toggled == false) {
    btn_toggle_system.toggleSpecialButton(SpecialLEDButton::SHIFT);
    toggle = true;
  }
}

// Step 3: Detect when button is no longer pressed aka released and set released to true
// ** detect button release **
released = true;

// Step 4: Only when button was pressed, toggled and released again, reset CurrentButtonState to all false so the button can be toggled again
if (pressed == true && released == true && toggled == true) {
    pressed = false;
    toggled = false;
    released = false;
}
```

## Technical Reference

### HID Report Structure

- **Input:** 22 bytes (ID=0x01, byte 3 = special buttons, SHIFT = bit 7 = 0x80)
- **Output:** 81 bytes (ID=0x80, bytes 17-24 = special LEDs, bytes 25-72 = matrix RGB)

### Key Enums

- `SpecialButton::SHIFT` (input detection)
- `SpecialLEDButton::SHIFT` (LED control)
- Conversion: `getSpecialButtonIndex()` maps LED enum to array index
