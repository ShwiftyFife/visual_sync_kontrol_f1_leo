# Traktor F1 7-Segment Display Module Implementation Draft

## Header Extension (led_controller.h)

Add these declarations to existing `led_controller.h`:

```cpp
// 7-Segment digit patterns lookup table
extern const uint8_t DIGIT_PATTERNS[11][7]; // 0-9 + blank

class DisplayController {
private:
    // Private helper function
    void setDisplaySegment(int display, int digit);

public:
    // 7-Segment Display Functions
    DisplayController setDisplayNumber(int number);
    DisplayController setDisplayDot(int display, bool on);
}
```

## Implementation (led_controller.cpp)

Add to existing `led_controller.cpp`:

```cpp
// 7-Segment digit patterns: [middle, lower_right, upper_right, top, upper_left, lower_left, bottom]
// Note: dot is handled separately
const uint8_t DIGIT_PATTERNS[11][7] = {
    {0, 127, 127, 127, 127, 127, 127},    // 0
    {0, 127, 127, 0, 0, 0, 0},            // 1
    {127, 0, 127, 127, 0, 127, 127},      // 2
    {127, 127, 127, 127, 0, 0, 127},      // 3
    {127, 127, 127, 0, 127, 0, 0},        // 4
    {127, 127, 0, 127, 127, 0, 127},      // 5
    {127, 127, 0, 127, 127, 127, 127},    // 6
    {0, 127, 127, 127, 0, 0, 0},          // 7
    {127, 127, 127, 127, 127, 127, 127},  // 8
    {127, 127, 127, 127, 127, 0, 127},    // 9
    {0, 0, 0, 0, 0, 0, 0}                 // 10 = blank
};

DisplayController::setDisplayNumber(int number) {
    // Clamp to valid range
    if (number < 1) number = 1;
    if (number > 99) number = 99;
    
    // Split number into digits
    int left_digit = (number >= 10) ? (number / 10) : 10; // 10 = blank index
    int right_digit = number % 10;
    
    // Set both displays
    DisplayController::setDisplaySegment(1, left_digit);  // Left display
    DisplayController::setDisplaySegment(2, right_digit); // Right display
}

DisplayController::setDisplayDot(int display, bool on) {
    uint8_t brightness = on ? 127 : 0;
    
    if (display == 1) {
        // Left display dot (byte 9)
        led_buffer[9] = brightness;
    } else if (display == 2) {
        // Right display dot (byte 1) 
        led_buffer[1] = brightness;
    }
    
    // Send updated buffer to device
    hid_write(hid_device, led_buffer, 81);
}

void DisplayController::setDisplaySegment(int display, int digit) {
    if (digit < 0 || digit > 10) return; // Invalid digit
    
    int base_byte;
    if (display == 1) {
        // Left display: bytes 10-16 (skip dot at byte 9)
        base_byte = 10;
    } else if (display == 2) {
        // Right display: bytes 2-8 (skip dot at byte 1)
        base_byte = 2;
    } else {
        return; // Invalid display
    }
    
    // Set 7 segments: middle, lower_right, upper_right, top, upper_left, lower_left, bottom
    for (int i = 0; i < 7; i++) {
        led_buffer[base_byte + i] = DIGIT_PATTERNS[digit][i];
    }
    
    // Send updated buffer to device
    hid_write(hid_device, led_buffer, 81);
}
```

## Usage Examples

```cpp
// In main loop or function calls:
DisplayController::setDisplayNumber(1);    // Shows " 1"
DisplayController::setDisplayNumber(42);   // Shows "42"
DisplayController::setDisplayNumber(99);   // Shows "99"
DisplayController::setDisplayNumber(150);  // Clamped to "99"

DisplayController::setDisplayDot(1, true);  // Left dot ON
DisplayController::setDisplayDot(2, false); // Right dot OFF
```

## Integration Notes

- **New Module:** `led_controller_display.h/.cpp` - keeps code organized
- **Dependencies:** Includes `led_controller.h` for access to `led_buffer[]` and `sendLEDBuffer()`
- **State Integration:** Uses existing LED buffer system, compatible with toggle system
- **Future Toggle:** Ready for `led_controller_toggle.cpp` integration later
- Digit patterns tested with standard 7-segment layout

## Required LED Controller Function

You may need to expose this function in `led_controller.h/.cpp` if it doesn't exist:

```cpp
void sendLEDBuffer(); // Sends current led_buffer to device via HID
```

## Byte Mapping Reference

- **Left Display:** bytes 9-16 (dot=9, middle=10, segments=11-16)
- **Right Display:** bytes 1-8 (dot=1, middle=2, segments=3-8)
- **Segment Order:** middle, lower_right, upper_right, top, upper_left, lower_left, bottom
