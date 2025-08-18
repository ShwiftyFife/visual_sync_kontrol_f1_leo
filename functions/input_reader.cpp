#include "input_reader.h"       // Include header file

#include <iostream>             // For std::cout and std::cerr
#include <iomanip>              // For std::hex (hexadecimal printing)
// #include <hidapi/hidapi.h>   // included already in header

// =============================================================================
// MAIN INPUT READING FUNCTION
// =============================================================================

/*
* Reads an input report from the Traktor Kontrol F1 device
* 
* @param device: Pointer to the opened HID device
* @param buffer: Array to store the 22-byte input report
* @return: true if read was successful, false if there was an error
*/

// Function:
bool readInputReport(hid_device *device, unsigned char *buffer) {

  // Step 1: Check if device is valid
  // Checks if pointers are valid before using them! This prevents crashes.
  if (device == nullptr) {
    std::cerr << "Error: Device is null (not connected any longer?)" << std::endl;
    return false;
  }

  // Step 2: Check if buffer is valid
  // Checks if pointers are valid before using them! This prevents crashes.
  if (buffer == nullptr) {
    std::cerr << "Error: Buffer is null" << std::endl;
    return false;
  }

  // Step 3: Try to read input report from the F1
  // hid_read() returns the number of bytes actually read
  int bytes_read = hid_read(device, buffer, INPUT_REPORT_SIZE);

  // Step 4: Check if the read operation was successfull
  if (bytes_read < 0) {
    std::cerr << "Error: Expected " << INPUT_REPORT_SIZE
              << " bytes, but got " << bytes_read << " bytes" << std::endl;
    return false;
  }
  
  // Step 6: Verify this is the correct type of report
  // The F1 always starts input reports with 0x01
  if (buffer[0] != INPUT_REPORT_ID) {
    std::cerr << "Error: Wrong report ID. Expected 0x"
              << std::hex << (int)INPUT_REPORT_ID
              << ", got 0x" << std::hex << (int)buffer[0] << std::endl;
    return false;
  }

  // Step 7: Success! There is a valid 22-byte input report
    return true;
}

// =============================================================================
// UTILITY FUNCTION FOR DEBUGGING
// =============================================================================

/*
* Prints the raw input report in hexadecimal format
* Useful for debugging and understanding what the F1 is sending
* 
* @param buffer: The 22-byte input report to print
*/

void printRawInputReport(const unsigned char* buffer) {
    std::cout << "Raw Input Report (22 bytes): ";
    
    // Loop through all 22 bytes and print each one in hex format
    for (int i = 0; i < INPUT_REPORT_SIZE; i++) {
        // Print each byte as a 2-digit hexadecimal number
        std::cout << std::hex << std::setfill('0') << std::setw(2) 
                  << (int)buffer[i] << " ";
    }
    
    std::cout << std::endl;  // New line at the end
}