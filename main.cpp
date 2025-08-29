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

	while (true) {
		// Read input report
		unsigned char input_report_buffer[INPUT_REPORT_SIZE];
		if (!readInputReport(device, input_report_buffer)) {
			std::cerr << "Error reading input report" << std::endl;
			continue;
		}

		// _________________________________________________________
		// Check if special buttons are pressed
		if (isSpecialButtonPressed(input_report_buffer, SpecialButton::SHIFT)) {
			std::cout << "SHIFT button is pressed" << std::endl;
			setSpecialButtonLED(SpecialLEDButton::SHIFT, 1.0f);
		}
		if (isSpecialButtonPressed(input_report_buffer, SpecialButton::REVERSE)) {
			std::cout << "REVERSE button is pressed" << std::endl;
			setSpecialButtonLED(SpecialLEDButton::REVERSE, 1.0f);
		}
		if (isSpecialButtonPressed(input_report_buffer, SpecialButton::TYPE)) {
			std::cout << "TYPE button is pressed" << std::endl;
			setSpecialButtonLED(SpecialLEDButton::TYPE, 1.0f);
		}
		if (isSpecialButtonPressed(input_report_buffer, SpecialButton::SIZE)) {
			std::cout << "SIZE button is pressed" << std::endl;
			setSpecialButtonLED(SpecialLEDButton::SIZE, 1.0f);
		}
		if (isSpecialButtonPressed(input_report_buffer, SpecialButton::BROWSE)) {
			std::cout << "BROWSE button is pressed" << std::endl;
			setSpecialButtonLED(SpecialLEDButton::BROWSE, 1.0f);
		}
		if (isSpecialButtonPressed(input_report_buffer, SpecialButton::SELECTOR_WHEEL)) {
			std::cout << "SELECTOR_WHEEL button is pressed" << std::endl;
		}

		// _________________________________________________________
		// Check if stop button is pressed
		if (isStopButtonPressed(input_report_buffer, StopButton::STOP1)) {
			std::cout << "STOP1 button is pressed" << std::endl;
			setStopButtonLED(StopLEDButton::STOP1, 1.0f);
		}
		if (isStopButtonPressed(input_report_buffer, StopButton::STOP2)) {
			std::cout << "STOP2 button is pressed" << std::endl;
			setStopButtonLED(StopLEDButton::STOP2, 1.0f);
		}
		if (isStopButtonPressed(input_report_buffer, StopButton::STOP3)) {
			std::cout << "STOP3 button is pressed" << std::endl;
			setStopButtonLED(StopLEDButton::STOP3, 1.0f);
		}
		if (isStopButtonPressed(input_report_buffer, StopButton::STOP4)) {
			std::cout << "STOP4 button is pressed" << std::endl;
			setStopButtonLED(StopLEDButton::STOP4, 1.0f);
		}

		// _________________________________________________________
		// Check if control button is pressed
		if (isControlButtonPressed(input_report_buffer, ControlButton::SYNC)) {
			std::cout << "SYNC button is pressed" << std::endl;
			setSpecialButtonLED(SpecialLEDButton::SYNC, 1.0f);
		}
		if (isControlButtonPressed(input_report_buffer, ControlButton::QUANT)) {
			std::cout << "QUANT button is pressed" << std::endl;
			setSpecialButtonLED(SpecialLEDButton::QUANT, 1.0f);
		}
		if (isControlButtonPressed(input_report_buffer, ControlButton::CAPTURE)) {
			std::cout << "CAPTURE button is pressed" << std::endl;
			setSpecialButtonLED(SpecialLEDButton::CAPTURE, 1.0f);
		}

		// _________________________________________________________
		// Check if Matrix buttons are pressed
		if (isMatrixButtonPressed(input_report_buffer, 1, 1)) {
			std::cout << "Matrix button (1,1) is pressed" << std::endl;
			setMatrixButtonLED(1, 1, LEDColor::red, 1.0f);
		}
		if (isMatrixButtonPressed(input_report_buffer, 1, 2)) {
			std::cout << "Matrix button (1,2) is pressed" << std::endl;
			setMatrixButtonLED(1, 2, LEDColor::lightorange, 1.0f);
		}
		if (isMatrixButtonPressed(input_report_buffer, 1, 3)) {
			std::cout << "Matrix button (1,3) is pressed" << std::endl;
			setMatrixButtonLED(1, 3, LEDColor::orange, 1.0f);
		}
		if (isMatrixButtonPressed(input_report_buffer, 1, 4)) {
			std::cout << "Matrix button (1,4) is pressed" << std::endl;
			setMatrixButtonLED(1, 4, LEDColor::warmyellow, 1.0f);
		}
		if (isMatrixButtonPressed(input_report_buffer, 2, 1)) {
			std::cout << "Matrix button (2,1) is pressed" << std::endl;
			setMatrixButtonLED(2, 1, LEDColor::yellow, 1.0f);
		}
		if (isMatrixButtonPressed(input_report_buffer, 2, 2)) {
			std::cout << "Matrix button (2,2) is pressed" << std::endl;
			setMatrixButtonLED(2, 2, LEDColor::lime, 1.0f);
		}
		if (isMatrixButtonPressed(input_report_buffer, 2, 3)) {
			std::cout << "Matrix button (2,3) is pressed" << std::endl;
			setMatrixButtonLED(2, 3, LEDColor::green, 1.0f);
		}
		if (isMatrixButtonPressed(input_report_buffer, 2, 4)) {
			std::cout << "Matrix button (2,4) is pressed" << std::endl;
			setMatrixButtonLED(2, 4, LEDColor::mint, 1.0f);
		}
		if (isMatrixButtonPressed(input_report_buffer, 3, 1)) {
			std::cout << "Matrix button (3,1) is pressed" << std::endl;
			setMatrixButtonLED(3, 1, LEDColor::cyan, 1.0f);
		}
		if (isMatrixButtonPressed(input_report_buffer, 3, 2)) {
			std::cout << "Matrix button (3,2) is pressed" << std::endl;
			setMatrixButtonLED(3, 2, LEDColor::turquise, 1.0f);
		}
		if (isMatrixButtonPressed(input_report_buffer, 3, 3)) {
			std::cout << "Matrix button (3,3) is pressed" << std::endl;
			setMatrixButtonLED(3, 3, LEDColor::blue, 1.0f);
		}
		if (isMatrixButtonPressed(input_report_buffer, 3, 4)) {
			std::cout << "Matrix button (3,4) is pressed" << std::endl;
			setMatrixButtonLED(3, 4, LEDColor::plum, 1.0f);
		}
		if (isMatrixButtonPressed(input_report_buffer, 4, 1)) {
			std::cout << "Matrix button (4,1) is pressed" << std::endl;
			setMatrixButtonLED(4, 1, LEDColor::violet, 1.0f);
		}
		if (isMatrixButtonPressed(input_report_buffer, 4, 2)) {
			std::cout << "Matrix button (4,2) is pressed" << std::endl;
			setMatrixButtonLED(4, 2, LEDColor::purple, 1.0f);
		}
		if (isMatrixButtonPressed(input_report_buffer, 4, 3)) {
			std::cout << "Matrix button (4,3) is pressed" << std::endl;
			setMatrixButtonLED(4, 3, LEDColor::magenta, 1.0f);
		}
		if (isMatrixButtonPressed(input_report_buffer, 4, 4)) {
			std::cout << "Matrix button (4,4) is pressed" << std::endl;
			setMatrixButtonLED(4, 4, LEDColor::fuchsia, 1.0f);
		}

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