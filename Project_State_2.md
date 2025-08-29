# Button Toggle System - Implementation Plan

## Overview

Implement a button toggle system that allows special buttons to toggle between their original brightness (from current scene) and 100% brightness. This creates a visual feedback system for button states while preserving scene colors/patterns.

## Core Concept

### Toggle Behavior

- **First press:** Button LED brightness → 1.0 (100%)
- **Second press:** Button LED brightness → original value (e.g., 0.1 from startup)
- **Scene change:** Save new original values, reset toggle states

## Design Structure - Struct-Based System

### File Structure

```{}
headers/btn_toggle_system.h
functions/btn_toggle_system.cpp
```

### Main Data Structure

```cpp
struct ButtonToggleSystem {
    // State storage arrays
    float original_special_brightness[8];    // Store original brightness values
    bool special_toggled[8];                 // Track which buttons are currently toggled
    
    // Core functions
    bool initialize();
    bool saveCurrentSpecialButtonState();
    bool toggleSpecialButton(SpecialLEDButton button);
};
```

### Array Mapping

Maps directly to `SpecialLEDButton` enum:

- Index 0 = BROWSE
- Index 1 = SIZE  
- Index 2 = TYPE
- Index 3 = REVERSE
- Index 4 = SHIFT
- Index 5 = CAPTURE
- Index 6 = QUANT
- Index 7 = SYNC

## Function Specifications

### `bool initialize()`

**Purpose:** Initialize the toggle system

**Implementation:**

- Initialize all toggle states to `false`
- Initialize brightness arrays to 0.0
- Called once at program startup

### `bool saveCurrentSpecialButtonState()`

**Purpose:** Read and store current LED brightness values as "original state"

**Implementation:**

- Read current brightness from led_controller buffer for each special button
- Store in `original_special_brightness[]` array
- Reset all toggle states to `false` (since we're saving new original state)
- Called after startup sequence, and after future scene changes

### `bool toggleSpecialButton(SpecialLEDButton button)`

**Purpose:** Toggle a specific special button between original and full brightness

**Implementation:**

- Convert enum to array index (0-7)
- Check current toggle state:
  - If `false` (original state): Set LED to 1.0 brightness, mark as toggled
  - If `true` (toggled state): Set LED back to original brightness, mark as not toggled
- Use existing `setSpecialButtonLED()` function from led_controller
- Return success/failure status

## Usage Flow

### Program Startup

```cpp
ButtonToggleSystem btn_toggle_system;

// 1. Initialize toggle system
btn_toggle_system.initialize();

// 2. Run startup sequence (sets LEDs)
startupSequence(device);

// (3. FUTURE,  BUT CRUTIAL FOR PLANNING)
setScene(scene_system.select(readSelectorWheelState());

// 3. Save the current state as "original"
btn_toggle_system.saveCurrentSpecialButtonState();
```

### Main Loop Integration

```cpp
// In main loop - detect button presses and toggle
unsigned char input_report_buffer[INPUT_REPORT_SIZE]; // INPUT_REPORT_SIZE = 22

if (readInputReport(device, input_report_buffer)) {
    // Check each special button for presses
    if (isSpecialButtonPressed(input_report_buffer, SpecialButton::SHIFT)) {
        btn_toggle_system.toggleSpecialButton(SpecialLEDButton::SHIFT);
    }
    
    if (isSpecialButtonPressed(input_report_buffer, SpecialButton::BROWSE)) {
        btn_toggle_system.toggleSpecialButton(SpecialLEDButton::BROWSE);
    }
    
    // ... repeat for other special buttons
}
```

### Future Scene Changes

This is a future module. This will be done later, but might be important for planning the toggle system.

```cpp
// When implementing scene switching (future):
// 1. Set new LED pattern
setMatrixButtonLED(1, 1, LEDColor::red, 0.5f);
setSpecialButtonLED(SpecialLEDButton::SHIFT, 0.3f);
// ... etc

// 2. Save new state as original
btn_toggle_system.saveCurrentSpecialButtonState();
```

## Implementation Details

### Toggle Logic Example

```cpp
bool toggleSpecialButton(SpecialLEDButton button) {
    // Convert enum to index
    int index = static_cast<int>(button);
    
    if (special_toggled[index]) {
        // Currently toggled ON → return to original
        setSpecialButtonLED(button, original_special_brightness[index]);
        special_toggled[index] = false;
    } else {
        // Currently in original state → toggle ON
        setSpecialButtonLED(button, 1.0f);
        special_toggled[index] = true;
    }
    
    return true;
}
```

### State Reading (Conceptual)

```cpp
bool saveCurrentSpecialButtonState() {
    // Read current brightness from LED controller buffer
    // (requires access to the LED buffer, provided as global variable from led_controller.cpp)
    // Details to be worked out during implementation
    
    for (int i = 0; i < 8; i++) {
        // Read current brightness for button i
        // Store in original_special_brightness[i]
        special_toggled[i] = false;  // Reset toggle state
    }
}
```

## Future Extensions

This struct-based design easily extends to:

### Matrix Button Toggles

```cpp
// Add to struct:
float original_matrix_brightness[4][4];
LEDColor original_matrix_colors[4][4];
bool matrix_toggled[4][4];

// Add function:
bool toggleMatrixButton(int row, int col);
```

### Stop Button Toggles

```cpp
// Add to struct:
float original_stop_brightness[4];
bool stop_toggled[4];

// Add function:
bool toggleStopButton(StopLEDButton button);
```

## Key Benefits

1. **Modular Design:** Separate module building on led_controller
2. **Simple Interface:** Easy to use from main loop
3. **Extensible:** Easy to add more button types
4. **Scene-Compatible:** Preserves scene colors, only changes brightness
5. **State Management:** Proper tracking of original vs toggled states

## Testing Strategy

1. **Start with one button type** (special buttons only)
2. **Test toggle behavior** - verify brightness changes correctly  
3. **Test state persistence** - verify original values are restored

This plan provides a solid foundation for the toggle system while remaining simple and extensible for future enhancements!
