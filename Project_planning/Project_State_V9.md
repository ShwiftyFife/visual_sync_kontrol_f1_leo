# Traktor Kontrol F1 Control Project - Current State

C++ project using HIDAPI to control a Traktor Kontrol F1 device. Read HID input reports and control LEDs on the F1. Built with CMake, developed in VS Code on macOS/Ubuntu.

## Project Structure

```{}
project/
├── main.cpp
├── functions/
│   ├── input_reader.cpp            Complete
│   ├── input_reader_wheel.cpp      Complete
│   ├── input_reader_knob.cpp       Complete
│   ├── input_reader_fader.cpp      Complete
│   ├── startup_sequence.cpp        Complete
│   ├── led_controller.cpp          Complete  
│   ├── led_controller_toggle.cpp   Complete + Zone System
│   ├── led_controller_display.cpp  Complete
│   └── led_scene_controller.cpp    Complete

├── headers/
│   ├── input_reader.h              Complete
│   ├── input_reader_wheel.h        Complete
│   ├── input_reader_knob.h         Complete
│   ├── input_reader_fader.h        Complete
│   ├── startup_sequence.h          Complete
│   ├── led_controller.h            Complete
│   ├── led_controller_toggle.h     Complete + Zone System
│   ├── led_controller_display.h    Complete
│   └── led_scene_controller.h      Complete
└── build/
    └── traktor                   Executable
```

## Completed Systems

### 1. Input Reader System - COMPLETE

- **Files:** `input_reader.h/cpp`
- **Capabilities:** Read 22-byte HID reports, detect all button presses
- **Functions:** `readInputReport()`, `isSpecialButtonPressed()`, `isStopButtonPressed()`, `isControlButtonPressed()`, `isMatrixButtonPressed()`
- **Enums:** `SpecialButton`, `StopButton`, `ControlButton`

### 2. LED Controller System - COMPLETE

- **Files:** `led_controller.h/cpp`  
- **Capabilities:** Control all F1 LEDs with 18-color system, persistent LED buffer, LED state storge
- **Functions:** `setMatrixButtonLED()`, `setSpecialButtonLED()`, `setControlButtonLED()`, `setStopButtonLED()`
- **Features:** BRG format conversion, 7-bit hardware conversion, automatic HID sending
- **Enums:** `LEDColor`, `SpecialLEDButton`, `ControlLEDButton`, `StopLEDButton`

### 3. Startup Sequence - COMPLETE

- **Files:** `startup_sequence.h/cpp`
- **Capabilities:** Diagonal wave animation across 4x4 matrix, sets initial LED scene

### 4. Zone-Based Button Toggle System - COMPLETE

- **Files:** `led_controller_toggle.h/cpp`
- **Capabilities:** Toggle any button between original brightness (and color for matrix) and 100% brightness with exclusive matrix zone control
- **Key Features:**
  - Press/release state tracking prevents rapid-fire toggling during button holds
  - Preserves original LED states for restoration
  - Works with all button types: special, control, stop, matrix
  - **Zone-based exclusive matrix toggling** - only one matrix button can be toggled per zone
  - **Configurable matrix zones** - FULL_MATRIX (all 16 buttons) or LEFT_RIGHT_SPLIT (columns 1-2 vs 3-4)
- **Functions:**
  - `toggleSpecialButton()`, `toggleControlButton()`, `toggleStopButton()`, `toggleMatrixButton()`
  - `shouldToggleXButton()` - handles press/release detection
  - `updateButtonStates()` - tracks button states between frames
  - `setMatrixToggleMode()`, `getMatrixToggleMode()` - configure zone behavior
  - `getButtonZone()`, `untoggleMatrixZone()`, `untoggleAllMatrixButtons()` - zone management
- **Arrays:** Uses 1-4 indexing consistently with `[5][5]` arrays for matrix buttons
- **Enums:** `MatrixToggleZone` (FULL_MATRIX, LEFT_RIGHT_SPLIT)

### 5. Input reader for Selector Wheel - COMPLETE

- **Files:** `input_reader_wheel.h/cpp`  
- **Capabilities:**  Read 22-byte HID reports, detect selector wheel state, detect clockwise or counter clockwise rotation
- **Functions:** `initialize()`, `calculateDirection()`, `checkWheelRotation()`

### 6. 7-Segment Display Controller - COMPLETE

- **Files:** `led_controller_display.h/cpp`  
- **Capabilities:** Control dual 7-segment displays, set numbers 0-99, set individual segments, set decimal points
- **Functions:** `setDisplayNumber()`, `setDisplaySegment()`, `setDisplayDot()`

### 7. Scene Controller System - COMPLETE

- **Files:** `scene_controller.h/cpp`
- **Capabilities:** Apply predefined LED scenes to 4x4 matrix based on effects page number
- **Functions:** `setEffectsPageScene()`, `applyScene1()`, `applyScene2()`, `applyDefaultScene()`

### 8. Input reader for Knobs and faders - COMPLETE

**Key Features:** LSB-first byte extraction from bytes 6-13 (knobs) and 14-21 (faders)

#### Knobs

- **Files:** `input_reader_knob.h/cpp`
- **Capabilities:** Read all 4 knobs with 12-bit precision, normalized 0.000-1.000 output, change detection
- **Functions:** `getKnobValue()`, `hasKnobChanged()`, `updateKnobStates()`, `getRawKnobValue()`, `printKnobValues()`

#### Faders

- **Files:** `input_reader_fader.h/cpp`
- **Capabilities:** Read all 4 faders with 12-bit precision, normalized 0.000-1.000 output, change detection
- **Functions:** `getFaderValue()`, `hasFaderChanged()`, `updateFaderStates()`, `getRawFaderValue()`, `printFaderValues()`

Functions: getKnobValue(), hasKnobChanged(), updateKnobStates(), getRawKnobValue(), printKnobValues()
Usage: Class-based with initialize/update pattern

## Technical Reference

### HID Communication

#### Input Byte Layout (22 bytes total)

- **Input:** 22 bytes (ID=0x01, button data in bytes 1-4, analog in bytes 5-21)

- Byte 0: Report ID (always 0x01)
- Byte 1: Matrix buttons (1,1) to (4,2) - bits 7-0
- Byte 2: Matrix buttons (1,3) to (4,4) - bits 7-0
- Byte 3: Special buttons - bits 7-0 (SHIFT=0x80, BROWSE=0x08, etc.)
- Byte 4: Stop buttons + control buttons - bits 7-0
- Byte 5: Selector wheel state 0 - 255
- Bytes 6-13: Knobs 1-4 (2 bytes each, LSB first)
- Bytes 14-21: Faders 1-4 (2 bytes each, LSB first)

#### Output Byte Layout (81 bytes total)

- **Output:** 81 bytes

- Byte 0: Report ID (always 0x80)
- Bytes 1-8: Right 7-segment display
- Bytes 9-16: Left 7-segment display
- Bytes 17-24: Special buttons (single brightness each)
- Bytes 25-72: Matrix RGB LEDs (16 buttons × 3 colors BRG format)
- Bytes 73-80: Stop buttons (2 LEDs per stop button)

### Matrix Button Layout

- 4x4 grid using 1-4 indexing (row 1-4, col 1-4)
- Input mapping: byte 1 covers (1,1)-(4,2), byte 2 covers (1,3)-(4,4)
- LED mapping: 3 bytes per button (BRG format), starting at byte 25

### Button Categories

- **Special buttons (5):** BROWSE, SIZE, TYPE, REVERSE, SHIFT
- **Control buttons (3):** CAPTURE, QUANT, SYNC  
- **Stop buttons (4):** STOP1, STOP2, STOP3, STOP4
- **Matrix buttons (16):** 4x4 grid with RGB LEDs

### Analog Controls

- **Knobs (4):** 12-bit precision, bytes 6-13, LSB first
- **Faders (4):** 12-bit precision, bytes 14-21, LSB first

### Zone-Based Matrix Toggle System

- **FULL_MATRIX mode:** All 16 matrix buttons compete - only one can be toggled at a time
- **LEFT_RIGHT_SPLIT mode:** Columns 1-2 and columns 3-4 act as independent zones
- **Auto-untoggle behavior:** Pressing a new button automatically untoggle others in the same zone
- **Same-button toggle:** Pressing an already-toggled button untoggle it (normal toggle behavior)

## Main Loop Pattern

```cpp
// =============================================================================
// F1 device identifiers
// =============================================================================
const unsigned short VENDOR_ID = 0x17cc;
const unsigned short PRODUCT_ID = 0x1120;

// =============================================================================
// START
// =============================================================================
int main() {

    // Initialize HIDAPI
    int res = hid_init();

    // Open the device:
    hid_device *device;                         // Declare pointer to HID device handle
    ButtonToggleSystem btn_toggle_system;       // Declare button toggle system
    WheelInputReader wheel_input_reader;        // Declare wheel reader system
    KnobInputReader knob_input_reader;          // Declare knob reader system
    FaderInputReader fader_input_reader;        // Declare fader reader system
    int current_effect_page = 1;                // Declare current effects page variable
    DisplayController display_controller;       // Declare display controller
    SceneController scene_controller;           // Declare LED scene controller


    // Open the device using the VendorID, ProductID
    device = hid_open(VENDOR_ID, PRODUCT_ID, NULL);

    initializeLEDController(device);            // Initialize the LED controller
    startupSequence(device);                    // Run startup sequence
    btn_toggle_system.initialize();             // Initialize button toggle system (sets FULL_MATRIX mode by default)
    wheel_input_reader.initialize();            // Initialize wheel input reader and set first page
    display_controller.setDisplayNumber(current_effect_page);   // Set first effects page on display
    display_controller.setDisplayDot(1, true);  // Turn on left dot to indicate page is loaded
    scene_controller.setEffectsPageScene(current_effect_page);  // Load first effects page scene

    // Optional: Set matrix toggle mode (default is FULL_MATRIX)
    // btn_toggle_system.setMatrixToggleMode(MatrixToggleZone::LEFT_RIGHT_SPLIT);

    // =============================================================================
    // WHILE TRUE LOOP
    // =============================================================================
    while (true) {
        readInputReport(device, input_data);
        
        // =======================================
        // Check for selector wheel rotation and set effects page
        // =======================================
        WheelDirection selector_wheel_direction = wheel_input_reader.checkWheelRotation(input_data);
        // ... check selector_wheel_direction and adjust effets page by +1 or -1 ...
        // ... select effects page with isSpecialButtonPressed, scene_controller.setEffectsPageScene, and btn_toggle_system.resetAllToggleStates ...

        // =======================================
        // Read and update Knob values
        // =======================================
        float knob_value_1 = knob_input_reader.getKnobValue(input_report_buffer, 1);
        float fader_value_1 = fader_input_reader.getFaderValue(input_report_buffer, 1);
        // ... read other knobs and faders ...
        // ... print knob and fader values to console ...

        // =======================================
        // Check for toggles (only triggers on press, not hold)
        // =======================================
        btn_toggle_system.shouldToggleSpecialButton(input_data, SpecialButton::SHIFT, SpecialLEDButton::SHIFT);
        // ... other button checks ...
        // ... matrix buttons with zone-based-exclusive toggling ...
        
        // Update button states for next frame
        btn_toggle_system.updateButtonStates(input_data);
    }

    hid_close(device);                          // Close the device
    res = hid_exit();
    return 0;
}
```

## Next Steps

1. **Scene controller zone integration:** Allow the scene controlelr to also set the matrix toggle mode based on effects page
