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

#include "headers/startup_sequence.h"					// Include startup effects module
#include "headers/input_reader.h"							// Include input read module
#include "headers/led_controller.h"						// Include LED control module
#include "headers/led_controller_toggle.h"		// Include LED controller toggle module


// F1 device identifiers (same as before)
const unsigned short VENDOR_ID = 0x17cc;
const unsigned short PRODUCT_ID = 0x1120;


int main() {

		// =============================================================================
		// START UP SEQUENCE
		// =============================================================================

		// create start up message
		std::cout << "" << std::endl;
		std::cout << "=== Starting Visual Sync Kontrol F1 ===" << std::endl;
		std::cout << "" << std::endl;

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
		// Declare button toggle system
		ButtonToggleSystem btn_toggle_system;

		// Open the device using the VendorID, ProductID, and optionally the Serial number.
		// If the device is opened successfully, the pointer will not be null.
		device = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
		if (device) {
				std::cout << "- Opening Traktor Kontrol F1..." << std::endl;

				// Initialize the LED controller
				initializeLEDController(device);

				// Run startup sequence
				startupSequence(device);

				// Initialize button toggle system
				btn_toggle_system.initialize();

				// Send success message
				std::cout << "" << std::endl;
				std::cout << "- Traktor Kontrol F1 opened successfully!" << std::endl;
		} else {
				// Send error message and close program
				std::cout << "- Unable to open device..." << std::endl;
				std::cout << "Shutting down..." << std::endl;
				hid_exit();
						return 0;
		}

		// Send startup message how to terminate while(true) loop
		std::cout << "" << std::endl;
		std::cout << "+++ Press Ctrl+C to exit. +++" << std::endl;
		std::cout << "" << std::endl;


		// =============================================================================
		// STUFFs HAPPENIN HERE
		// =============================================================================


		while (true) {
				// Read input report
				unsigned char input_report_buffer[INPUT_REPORT_SIZE];
				if (!readInputReport(device, input_report_buffer)) {
						std::cerr << "Error, shutting down..." << std::endl;
						return -1;
				}

				// Check for special button toggles (only triggers on press, not hold)
				btn_toggle_system.shouldToggleSpecialButton(input_report_buffer, SpecialButton::SHIFT, SpecialLEDButton::SHIFT);
				btn_toggle_system.shouldToggleSpecialButton(input_report_buffer, SpecialButton::REVERSE, SpecialLEDButton::REVERSE);
				btn_toggle_system.shouldToggleSpecialButton(input_report_buffer, SpecialButton::TYPE, SpecialLEDButton::TYPE);
				btn_toggle_system.shouldToggleSpecialButton(input_report_buffer, SpecialButton::SIZE, SpecialLEDButton::SIZE);
				btn_toggle_system.shouldToggleSpecialButton(input_report_buffer, SpecialButton::BROWSE, SpecialLEDButton::BROWSE);

				// Check for control button toggles
				btn_toggle_system.shouldToggleControlButton(input_report_buffer, ControlButton::CAPTURE, ControlLEDButton::CAPTURE);
				btn_toggle_system.shouldToggleControlButton(input_report_buffer, ControlButton::QUANT, ControlLEDButton::QUANT);
				btn_toggle_system.shouldToggleControlButton(input_report_buffer, ControlButton::SYNC, ControlLEDButton::SYNC);

				// Check for stop button toggles
				btn_toggle_system.shouldToggleStopButton(input_report_buffer, StopButton::STOP1, StopLEDButton::STOP1);
				btn_toggle_system.shouldToggleStopButton(input_report_buffer, StopButton::STOP2, StopLEDButton::STOP2);
				btn_toggle_system.shouldToggleStopButton(input_report_buffer, StopButton::STOP3, StopLEDButton::STOP3);
				btn_toggle_system.shouldToggleStopButton(input_report_buffer, StopButton::STOP4, StopLEDButton::STOP4);

				// Check for matrix button toggles
				for (int row = 1; row <= 4; row++) {
					for (int col = 1; col <= 4; col++) {
						btn_toggle_system.shouldToggleMatrixButton(input_report_buffer, row, col);
					}
				}

				// Update button states for next frame
				btn_toggle_system.updateButtonStates(input_report_buffer);
		}

	// =============================================================================
	// CODE CLOSES
	//=============================================================================
	// Close the device
	hid_close(device);

	// Finalize the hidapi library
	res = hid_exit();

	return 0;
}