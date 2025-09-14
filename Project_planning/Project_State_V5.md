# Traktor Kontrol F1 Control Project - Current State

C++ project using HIDAPI to control a Traktor Kontrol F1 device. Read HID input reports and control LEDs on the F1. Built with CMake, developed in VS Code on macOS/Ubuntu.

## Current Status: TOGGLE SYSTEM COMPLETE

All core systems working: input reading, LED control, startup sequence, and button toggle system.

## Project Structure

```{}
project/
├── main.cpp
├── functions/
│   ├── startup_sequence.cpp      Complete
│   ├── led_controller.cpp        Complete  
│   ├── input_reader.cpp          Complete
│   └── led_controller_toggle.cpp Complete - NEW!
├── headers/
│   ├── startup_sequence.h        Complete
│   ├── led_controller.h          Complete
│   ├── input_reader.h            Complete  
│   └── led_controller_toggle.h   Complete - NEW!
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

### 4. Button Toggle System - NEW

- **Files:** `led_controller_toggle.h/cpp`
- **Capabilities:** Toggle any button between original brightness (and color for matrix) and 100% brightness
- **Key Features:**
  - Press/release state tracking prevents rapid-fire toggling during button holds
  - Preserves original LED states for restoration
  - Works with all button types: special, control, stop, matrix
- **Functions:**
  - `toggleSpecialButton()`, `toggleControlButton()`, `toggleStopButton()`, `toggleMatrixButton()`
  - `shouldToggleXButton()` - handles press/release detection
  - `updateButtonStates()` - tracks button states between frames
- **Arrays:** Uses 1-4 indexing consistently with `[5][5]` arrays for matrix buttons

## Technical Reference

### HID Communication

#### Input Byte Layout (22 bytes total)

- **Input:** 22 bytes (ID=0x01, button data in bytes 1-4, analog in bytes 5-21)

- Byte 0: Report ID (always 0x01)
- Byte 1: Matrix buttons (1,1) to (4,2) - bits 7-0
- Byte 2: Matrix buttons (1,3) to (4,4) - bits 7-0
- Byte 3: Special buttons - bits 7-0 (SHIFT=0x80, BROWSE=0x08, etc.)
- Byte 4: Stop buttons + control buttons - bits 7-0
- Bytes 5-21: Knobs, faders, selector wheel (analog values)

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

    // Open the device using the VendorID, ProductID
    device = hid_open(VENDOR_ID, PRODUCT_ID, NULL);

    initializeLEDController(device);            // Initialize the LED controller
    startupSequence(device);                    // Run startup sequence
    btn_toggle_system.initialize();             // Initialize button toggle system

    // =============================================================================
    // WHILE TRUE LOOP
    // =============================================================================
    while (true) {
        readInputReport(device, input_data);
        
        // Check for toggles (only triggers on press, not hold)
        btn_toggle_system.shouldToggleSpecialButton(input_data, SpecialButton::SHIFT, SpecialLEDButton::SHIFT);
        // ... other button checks ...
        
        // Update button states for next frame
        btn_toggle_system.updateButtonStates(input_data);
    }

    hid_close(device);                          // Close the device
    res = hid_exit();
    return 0;
}
```

## Key Lessons Learned

## Next Steps

1. **7-Segment Display:** Show numbers/text on the dual 7-segment displays
2. **Scene System:** Save/restore complete LED states using selector wheel
3. **Fader/Knob Input:** Read analog values from faders and knobs
4. **Arduino Communication:** Send control signals to external Arduino
