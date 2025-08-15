#include "headers/startup_effect.h"
#include <stdio.h>       // stdio.h gives printf() — basic text output (like print() in R).
#include <unistd.h>      // unistd.h gives usleep() — sleep for microseconds
#include <cstring>       // cstring gives memset() — memory manipulation


// Green LED positions for the 4x4 matrix (reading left to right, top to bottom)
// Based on BRG format: Blue, Red, Green for each button
static const int green_positions[16] = {
  27, // (1,1) green
  30, // (2,1) green  
  33, // (3,1) green
  36, // (4,1) green
  39, // (1,2) green
  42, // (2,2) green
  45, // (3,2) green
  48, // (4,2) green
  51, // (1,3) green
  54, // (2,3) green
  57, // (3,3) green
  60, // (4,3) green
  63, // (1,4) green
  66, // (2,4) green
  69, // (3,4) green
  72  // (4,4) green
};

// ________________________________________________________________
// Clear all LEDs
void clearAllLEDs(hid_device *handle) {
  if (!handle) {
    printf("Error: Invalid device handle\n");
    return;
  }

  // Prepare the buffer of length 81
  unsigned char buf[81] = {0}; // Initialize all bytes to 0
  buf[0] = 0x80; // Set report ID

  // Send the command
  int res = hid_write(handle, buf, sizeof(buf));
  if (res < 0) {
    printf("Error clearing LEDs: %ls\n", hid_error(handle));
  }
}


// ________________________________________________________________
void startupLEDSequence(hid_device *handle) {
    if (!handle) {
        printf("Error: Invalid device handle for startup sequence\n");
        return;
    }
    
    printf("Running startup LED sequence...\n");

    // Prepare the buffer of length 81
    unsigned char buf[81] = {0}; // Initialize all bytes to 0
    buf[0] = 0x80; // Set report ID
    

    // Turn on each green LED sequentially
    for (int i = 0; i < 16; i++) {
        // Clear previous LED (turn off all LEDs)
        memset(&buf[1], 0, 80); // Clear bytes 1-80, keep report ID
        
        // Turn on current green LED at full brightness
        buf[green_positions[i]] = 127;
        
        // Send the command
        int res = hid_write(handle, buf, sizeof(buf));
        if (res < 0) {
            printf("Error writing to device during startup: %ls\n", hid_error(handle));
            return;
        }
        
        // Wait before moving to next LED (100ms)
        usleep(100000);
    }
    
    // Turn off all LEDs at the end
    clearAllLEDs(handle);

    printf("Startup LEDs completed!\n");
}