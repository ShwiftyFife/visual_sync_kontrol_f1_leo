#include "headers/input_reader_wheel.h"
#include "headers/input_reader.h"

#include <iostream>             // For std::cout and std::cerr
#include <iomanip>              // For std::hex (hexadecimal printing)
#include <cmath>                // For std::abs
// #include <hidapi/hidapi.h>   // included already in header

// =============================================================================
// WHEEL INPUT READER CLASS IMPLEMENTATION
// =============================================================================

/*
* Constructor/ Initialization
*
* Initializes the wheel input reader and tracking variables.
*/
bool WheelInputReader::initialize() {
    last_wheel_value = 0;
    initialized = false;
    return true;
}

// =============================================================================
// MAIN WheelInputReader CLASS FUNCTIONS
// =============================================================================

/*
* Updates the wheel state with the current input buffer
* Call this every frame to track wheel changes
* 
* @param buffer: The 22-byte input report from readInputReport()
*/
void WheelInputReader::updateWheelState(const unsigned char* buffer) {
    // Step 1: Check if device is valid
    // Checks if pointers are valid before using them! This prevents crashes.
    if (buffer == nullptr) {
        std::cerr << "WheelInputReader Error: Buffer is null" << std::endl;
        return;
    }

    // Step 2: Read the current wheel value from its position in the buffer
    unsigned char current_value = buffer[WHEEL_BYTE_POSITION];

    // Step 3: Update the last wheel value, set initialized to true to indicate, that wheel state is beeing tracked
    if (!initialized) {
        last_wheel_value = current_value;
        initialized = true;
    } else {
        last_wheel_value = current_value;
    }
}

/*
* Checks if the wheel has rotated since last update
* 
* @return: true if wheel position changed, false otherwise

bool WheelInputReader::hasWheelRotated() const {
    // We can't detect rotation without at least one previous reading
    if (!initialized) {
        return false;
    }
    
    // This will be implemented in getWheelDirection() for efficiency
    // For now, just return false - the real logic is in getWheelDirection
    return false;
}
*/

/*
* Determines the direction of wheel rotation
* Handles wraparound cases (0xFF -> 0x00 and 0x00 -> 0xFF)
* 
* @param buffer: The 22-byte input report from readInputReport()
* @return: WheelDirection enum indicating rotation direction
*/
WheelDirection WheelInputReader::getWheelDirection(const unsigned char* buffer) {
    // Step 1: Check if device is valid
    if (buffer == nullptr) {
        std::cerr << "WheelInputReader Error: Buffer is null in getWheelDirection()" << std::endl;
        return WheelDirection::NONE;
    }

    // Step 2: Check if wheel state is initialized. If not set first reading
    if (!initialized) {
        // First reading - establish baseline
        updateWheelState(buffer);
        return WheelDirection::NONE;
    }
    
    // Step 3: Read current wheel value
    unsigned char current_value = buffer[WHEEL_BYTE_POSITION];

    // Step 4: Set previous_value for comparison from last value recorded
    unsigned char previous_value = last_wheel_value;
    
    // Step 5: Update state for next time
    last_wheel_value = current_value;

    // Step 6: Check for no change
    if (current_value == previous_value) {
        return WheelDirection::NONE;
    }

    // Step 7: Calculate distance and handle wraparound
    // Calculate the difference
    int difference;
    // Check for wraparound cases
    if (previous_value > 200 && current_value < 50) {
        // Wraparound: 0xFF -> 0x00 (clockwise)
        difference = (current_value + 256) - previous_value;
    } else if (previous_value < 50 && current_value > 200) {
        // Wraparound: 0x00 -> 0xFF (counter-clockwise)
        difference = current_value - (previous_value + 256);
    } else {
        // Normal case
        difference = current_value - previous_value;
    }

    // Step 8:
    // Determine direction based on difference
    if (difference > 0) {
        return WheelDirection::CLOCKWISE;
    } else if (difference < 0) {
        return WheelDirection::COUNTER_CLOCKWISE;
    } else {
        return WheelDirection::NONE;
    }
}

// =============================================================================
// UTILITY FUNCTION FOR DEBUGGING
// =============================================================================

/*
* Prints wheel debug information
* Useful for development and troubleshooting
* 
* @param buffer: The 22-byte input report from readInputReport()
*/
void WheelInputReader::printWheelDebugInfo(const unsigned char* buffer) const {
    // Step 1: Check if buffer is valid
    if (buffer == nullptr) {
        std::cout << "WheelDebug: Buffer is null" << std::endl;
        return;
    }
    
    unsigned char current = buffer[WHEEL_BYTE_POSITION];
    
    std::cout << "WheelDebug: CurrentValue=0x" << std::hex << std::setfill('0') << std::setw(2) 
              << (int)current << " (" << std::dec << (int)current << ")";
    
    if (initialized) {
        std::cout << ", LastValue=0x" << std::hex << std::setfill('0') << std::setw(2) 
                  << (int)last_wheel_value << " (" << std::dec << (int)last_wheel_value << ")";
                  
        // Calculate difference for debug info
        int diff = current - last_wheel_value;
        std::cout << ", Diff=" << std::dec << diff;
    } else {
        std::cout << ", NotInitialized";
    }
    
    std::cout << std::endl;
}