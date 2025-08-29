#include "headers/startup_sequence.h"       // Include header file
#include "headers/led_controller.h"         // Include LED controller header file

#include <iostream>      // iostream gives std::cout
#include <unistd.h>      // unistd.h gives usleep() — sleep for microseconds
#include <cstring>       // cstring gives memset() — memory manipulation
#include <thread>               // For std::this_thread::sleep_for (modern C++ sleep)
#include <chrono>               // For std::chrono::milliseconds
// #include <hidapi/hidapi.h>   // included already in header


// =============================================================================
// START UP SEQUENCE
// =============================================================================

/*
* Runs a LED wave animation on the F1 matrix buttons
* Creates a diagonal wave pattern that spreads across the 4x4 matrix
* 
* @param device: Pointer to the opened HID device
*/

void startupSequence(hid_device* device) {
    
    // Step 1: Check if device is valid
    if (device == nullptr) {
        std::cerr << "Error: Invalid device handle for startup sequence" << std::endl;
        return;
    }
    
    // Step 2: Animation timing configuration
    const int step_delay_ms = 50;  // 50ms between each animation step
    
    // Step 3: Start the animation
    std::cout << "  - Running startup LED sequence..." << std::endl;
    
    // Animation creates a diagonal wave pattern:
    // The pattern moves from bottom-right to top-left in diagonal waves
    // Each step lights up LEDs along diagonal lines, creating a wave effect
    
    // =============================================================================
    // ANIMATION STEP 1-2: First diagonal
    // =============================================================================
    
    // Step 1: Start with single LED at (4,4) - dim green
    setMatrixButtonLED(4, 4, LEDColor::green, 0.5f);
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));
    
    // Step 2:
    setMatrixButtonLED(4, 4, LEDColor::green, 1.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));
    
    // =============================================================================
    // ANIMATION STEP 3-4: Second diagonal
    // =============================================================================
    
    // Step 3:
    setMatrixButtonLED(3, 4, LEDColor::green, 0.5f);
    setMatrixButtonLED(4, 3, LEDColor::green, 0.5f);
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));
    
    // Step 4:
    setMatrixButtonLED(3, 4, LEDColor::green, 1.0f);
    setMatrixButtonLED(4, 3, LEDColor::green, 1.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));
    
    // =============================================================================
    // ANIMATION STEP 5-6: Third diagonal
    // =============================================================================
    
    // Step 5:
    setMatrixButtonLED(4, 4, LEDColor::green, 0.5f);  // Fade corner
    setMatrixButtonLED(2, 4, LEDColor::green, 0.5f);  // New LEDs dim
    setMatrixButtonLED(3, 3, LEDColor::green, 0.5f);
    setMatrixButtonLED(4, 2, LEDColor::green, 0.5f);
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));
    
    // Step 6:
    turnOffMatrixButtonLED(4, 4);  // Turn off corner
    setMatrixButtonLED(2, 4, LEDColor::green, 1.0f);
    setMatrixButtonLED(3, 3, LEDColor::green, 1.0f);
    setMatrixButtonLED(4, 2, LEDColor::green, 1.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));
    
    // =============================================================================
    // ANIMATION STEP 7-8: Fourth diagonal (main diagonal)
    // =============================================================================
    
    // Step 7:
    setMatrixButtonLED(3, 4, LEDColor::green, 0.5f);  // Fade second diagonal
    setMatrixButtonLED(4, 3, LEDColor::green, 0.5f);
    setMatrixButtonLED(1, 4, LEDColor::green, 0.5f);  // New main diagonal dim
    setMatrixButtonLED(2, 3, LEDColor::green, 0.5f);
    setMatrixButtonLED(3, 2, LEDColor::green, 0.5f);
    setMatrixButtonLED(4, 1, LEDColor::green, 0.5f);
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));
    
    // Step 8:
    turnOffMatrixButtonLED(3, 4);
    turnOffMatrixButtonLED(4, 3);
    setMatrixButtonLED(1, 4, LEDColor::green, 1.0f);
    setMatrixButtonLED(2, 3, LEDColor::green, 1.0f);
    setMatrixButtonLED(3, 2, LEDColor::green, 1.0f);
    setMatrixButtonLED(4, 1, LEDColor::green, 1.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));
    
    // =============================================================================
    // ANIMATION STEP 9-10: Fifth diagonal
    // =============================================================================
    
    // Step 9:
    setMatrixButtonLED(2, 4, LEDColor::green, 0.5f);  // Fade third diagonal
    setMatrixButtonLED(3, 3, LEDColor::green, 0.5f);
    setMatrixButtonLED(4, 2, LEDColor::green, 0.5f);
    setMatrixButtonLED(1, 3, LEDColor::green, 0.5f);  // New fifth diagonal dim
    setMatrixButtonLED(2, 2, LEDColor::green, 0.5f);
    setMatrixButtonLED(3, 1, LEDColor::green, 0.5f);
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));
    
    // Step 10:
    turnOffMatrixButtonLED(2, 4);  // Turn off third diagonal
    turnOffMatrixButtonLED(3, 3);
    turnOffMatrixButtonLED(4, 2);
    setMatrixButtonLED(1, 3, LEDColor::green, 1.0f);
    setMatrixButtonLED(2, 2, LEDColor::green, 1.0f);
    setMatrixButtonLED(3, 1, LEDColor::green, 1.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));
    
    // =============================================================================
    // ANIMATION STEP 11-12: Sixth diagonal
    // =============================================================================
    
    // Step 11:
    setMatrixButtonLED(1, 4, LEDColor::green, 0.5f);  // Fade main diagonal
    setMatrixButtonLED(2, 3, LEDColor::green, 0.5f);
    setMatrixButtonLED(3, 2, LEDColor::green, 0.5f);
    setMatrixButtonLED(4, 1, LEDColor::green, 0.5f);
    setMatrixButtonLED(1, 2, LEDColor::green, 0.5f);  // New sixth diagonal dim
    setMatrixButtonLED(2, 1, LEDColor::green, 0.5f);
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));
    
    // Step 12:
    turnOffMatrixButtonLED(1, 4);
    turnOffMatrixButtonLED(2, 3);
    turnOffMatrixButtonLED(3, 2);
    turnOffMatrixButtonLED(4, 1);
    setMatrixButtonLED(1, 2, LEDColor::green, 1.0f);
    setMatrixButtonLED(2, 1, LEDColor::green, 1.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));
    
    // =============================================================================
    // ANIMATION STEP 13-14: Seventh diagonal (top-left corner)
    // =============================================================================
    
    // Step 13:
    setMatrixButtonLED(1, 3, LEDColor::green, 0.5f);  // Fade fifth diagonal
    setMatrixButtonLED(2, 2, LEDColor::green, 0.5f);
    setMatrixButtonLED(3, 1, LEDColor::green, 0.5f);
    setMatrixButtonLED(1, 1, LEDColor::green, 0.5f);  // Final corner dim
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));
    
    // Step 14:
    turnOffMatrixButtonLED(1, 3);
    turnOffMatrixButtonLED(2, 2);
    turnOffMatrixButtonLED(3, 1);
    setMatrixButtonLED(1, 1, LEDColor::green, 1.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));
    
    // =============================================================================
    // ANIMATION STEP 15-18: Wave fade out
    // =============================================================================
    
    // Step 15:
    setMatrixButtonLED(1, 2, LEDColor::green, 0.5f);
    setMatrixButtonLED(2, 1, LEDColor::green, 0.5f);
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));
    
    // Step 16:
    turnOffMatrixButtonLED(1, 2);
    turnOffMatrixButtonLED(2, 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));
    
    // Step 17: Final fade - corner dims
    setMatrixButtonLED(1, 1, LEDColor::green, 0.5f);
    std::this_thread::sleep_for(std::chrono::milliseconds(step_delay_ms));

    // Step 18: Final fade - all dims
    turnOffMatrixButtonLED(1, 1);

    // =============================================================================
    // FINAL STATE: Turn on all LEDs at specified brightness
    // =============================================================================
    
    std::cout << "  - Animation complete, setting final LED state..." << std::endl;
    
    // Clear all LEDs first
    clearAllLEDs();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Set all special button LEDs to 50% (browse 25%) brightness
    setSpecialButtonLED(SpecialLEDButton::BROWSE, 0.1f);
    setSpecialButtonLED(SpecialLEDButton::SIZE, 0.1f);
    setSpecialButtonLED(SpecialLEDButton::TYPE, 0.1f);
    setSpecialButtonLED(SpecialLEDButton::REVERSE, 0.1f);
    setSpecialButtonLED(SpecialLEDButton::SHIFT, 0.1f);
    setSpecialButtonLED(SpecialLEDButton::CAPTURE, 0.1f);
    setSpecialButtonLED(SpecialLEDButton::QUANT, 0.1f);
    setSpecialButtonLED(SpecialLEDButton::SYNC, 0.1f);

    // Set all stop button LEDs to 50% brightness
    setStopButtonLED(StopLEDButton::STOP1, 0.1f);
    setStopButtonLED(StopLEDButton::STOP2, 0.1f);
    setStopButtonLED(StopLEDButton::STOP3, 0.1f);
    setStopButtonLED(StopLEDButton::STOP4, 0.1f);

    // Set matrix LEDs to 25% brightness
    setMatrixButtonLED(1, 1, LEDColor::red, 0.2f);
    setMatrixButtonLED(1, 2, LEDColor::orange, 0.2f);
    setMatrixButtonLED(1, 3, LEDColor::lightorange, 0.2f);
    setMatrixButtonLED(1, 4, LEDColor::warmyellow, 0.2f);
    setMatrixButtonLED(2, 1, LEDColor::yellow, 0.2f);
    setMatrixButtonLED(2, 2, LEDColor::lime, 0.2f);
    setMatrixButtonLED(2, 3, LEDColor::green, 0.2f);
    setMatrixButtonLED(2, 4, LEDColor::mint, 0.2f);
    setMatrixButtonLED(3, 1, LEDColor::cyan, 0.2f);
    setMatrixButtonLED(3, 2, LEDColor::turquise, 0.2f);
    setMatrixButtonLED(3, 3, LEDColor::blue, 0.2f);
    setMatrixButtonLED(3, 4, LEDColor::plum, 0.2f);
    setMatrixButtonLED(4, 1, LEDColor::violet, 0.2f);
    setMatrixButtonLED(4, 2, LEDColor::purple, 0.2f);
    setMatrixButtonLED(4, 3, LEDColor::magenta, 0.2f);
    setMatrixButtonLED(4, 4, LEDColor::fuchsia, 0.2f);

    std::cout << "  - Startup sequence completed!" << std::endl;
}