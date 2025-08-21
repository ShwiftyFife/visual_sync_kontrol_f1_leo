// On Linux: build with control+shift+b (builds with cmake, declared in tasks.json)
// On Mac: build with command+shift+b (builds with cmake, declared in tasks.json)
// run in terminal with ./main


#include <iostream>			// For standard input output operations
#include <stdio.h> 			// stdio.h gives printf() — basic text output (like print() in R).
#include <wchar.h> 			// wchar.h gives wide-char support (wchar_t) used because USB string descriptors are UTF-16/wide.
#include <unistd.h> 		// unistd.h gives access to the POSIX operating system API.
#include <stdint.h> 		// stdint.h gives uint8_t support. uint8_t is an unsigned 8-bit integer type.
#include <hidapi/hidapi.h>  // HIDAPI library for USB HID device access
#include <fcntl.h>          // fcntl.h gives access to the POSIX file control API.
#include <cstring>          // string.h gives access to C-style string functions.
#include <chrono>
#include <thread>

#include "headers/startup_sequence.h"			// Include startup effects module
#include "headers/input_reader.h"				// Include input read module
#include "headers/led_controller.h"				// Include LED control module

// F1 device identifiers (same as before)
const unsigned short VENDOR_ID = 0x17cc;
const unsigned short PRODUCT_ID = 0x1120;


int main() {

	// =============================================================================
	// START UP SEQUENCE
	// =============================================================================

	// create start up message
	std::cout << "=== Starting Visual Sync Kontrol F1 ===" << std::endl;

	// Initialize HIDAPI, get return code, send error message if initialization fails
	int res = hid_init();
	if (res != 0) {
		std::cout << "- Failed to initialize HIDAPI" << std::endl;
		return 1;
	} else {
		std::cout << "- HID_API initialized successfully!" << std::endl;
	}

	// Open the device:
	// Declare pointer to HID device handle
	hid_device *device; 

	// Open the device using the VendorID, ProductID, and optionally the Serial number.
	// If the device is opened successfully, the pointer will not be null.
	device = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
	if (device) {
		std::cout << "- Opening Traktor Kontrol F1..." << std::endl;

		// Initialize the LED controller
		initializeLEDController(device);

		// Run startup sequence
		startupSequence(device);

		// Send success message
		std::cout << "- Traktor Kontrol F1 opened successfully!" << std::endl;
		std::cout << " -Press Ctrl+C to exit." << std::endl;
	} else {
		// Send error message and close program
		std::cout << "- Unable to open device..." << std::endl;
		std::cout << "Shutting down..." << std::endl;
		hid_exit();
				return 0;
	}

	// =============================================================================
	// STUFFs HAPPENIN HERE
	// =============================================================================

	


	// =============================================================================
	// CODE CLOSES
	//=============================================================================
	// Close the device
	hid_close(device);

	// Finalize the hidapi library
	res = hid_exit();

	return 0;
}