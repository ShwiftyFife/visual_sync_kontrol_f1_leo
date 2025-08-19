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

#include "headers/startup_effect.h"			// Include startup effects module
#include "headers/color_system.h"				// Include color system module
#include "headers/set_matrix_led.h"			// Include matrix LED module
#include "headers/input_reader.h"				// Include input read module

// F1 device identifiers (same as before)
const unsigned short VENDOR_ID = 0x17cc;
const unsigned short PRODUCT_ID = 0x1120;


int main() {

	// =============================================================================
	// START UP SEQUENCE
	// =============================================================================

	// create start up message
	printf("=== Starting Visual Sync Kontrol F1 ===\n\n");

	// Initialize HIDAPI return code, send error message if initialization fails
	int res = hid_init();
	if (res != 0) {
		printf("- Failed to initialize HIDAPI\n");
		return 1;
	} else {
		printf("- HID_API initialized successfully!\n\n");
	}

	// Open the device:
	// Declare pointer to HID device handle
	hid_device *device; 

	// Open the device using the VendorID, ProductID, and optionally the Serial number.
	device = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
	if (device) {
		printf("- Opening Traktor Kontrol F1...\n\n");
		startupLEDSequence(device);
		printf("- Traktor Kontrol F1 opened successfully!\n");
		printf(" -Press Ctrl+C to exit.\n\n");
	} else {
		printf("- Unable to open device...\nShutting down...\n");
		hid_exit();
				return 0;
	}
	// =============================================================================
	// STUFFs HAPPENIN HERE
	// =============================================================================
	
	MatrixLEDController matrixController(device);


	while (true) {
		// Read input report
		unsigned char buffer[INPUT_REPORT_SIZE];
		if (!readInputReport(device, buffer)) {
			std::cerr << "Error reading input report" << std::endl;
			continue;
		}

		// _________________________________________________________
		// Check if special buttons are pressed
		if (isSpecialButtonPressed(buffer, SpecialButton::SHIFT)) {
			std::cout << "SHIFT button is pressed" << std::endl;
		}
		if (isSpecialButtonPressed(buffer, SpecialButton::REVERSE)) {
			std::cout << "REVERSE button is pressed" << std::endl;
		}
		if (isSpecialButtonPressed(buffer, SpecialButton::TYPE)) {
			std::cout << "TYPE button is pressed" << std::endl;
		}
		if (isSpecialButtonPressed(buffer, SpecialButton::SIZE)) {
			std::cout << "SIZE button is pressed" << std::endl;
		}
		if (isSpecialButtonPressed(buffer, SpecialButton::BROWSE)) {
			std::cout << "BROWSE button is pressed" << std::endl;
		}
		if (isSpecialButtonPressed(buffer, SpecialButton::SELECTOR_WHEEL)) {
			std::cout << "SELECTOR_WHEEL button is pressed" << std::endl;
		}

		// _________________________________________________________
		// Check if stop button is pressed
		if (isStopButtonPressed(buffer, StopButton::STOP1)) {
			std::cout << "STOP1 button is pressed" << std::endl;
		}
		if (isStopButtonPressed(buffer, StopButton::STOP2)) {
			std::cout << "STOP2 button is pressed" << std::endl;
		}
		if (isStopButtonPressed(buffer, StopButton::STOP3)) {
			std::cout << "STOP3 button is pressed" << std::endl;
		}
		if (isStopButtonPressed(buffer, StopButton::STOP4)) {
			std::cout << "STOP4 button is pressed" << std::endl;
		}

		// _________________________________________________________
		// Check if control button is pressed
		if (isControlButtonPressed(buffer, ControlButton::SYNC)) {
			std::cout << "SYNC button is pressed" << std::endl;
		}
		if (isControlButtonPressed(buffer, ControlButton::QUANT)) {
			std::cout << "QUANT button is pressed" << std::endl;
		}
		if (isControlButtonPressed(buffer, ControlButton::CAPTURE)) {
			std::cout << "CAPTURE button is pressed" << std::endl;
		}

		// _________________________________________________________
		// Check if Matrix buttons are pressed
		if (isMatrixButtonPressed(buffer, 1, 1)) {
			std::cout << "Matrix button (1,1) is pressed" << std::endl;
			matrixController.setMatrixButtonLED(MatrixButton::matrix11, ColorSystem::red, 0.5f);
		}
		if (isMatrixButtonPressed(buffer, 1, 2)) {
			std::cout << "Matrix button (1,2) is pressed" << std::endl;
			matrixController.setMatrixButtonLED(MatrixButton::matrix12, ColorSystem::orange, 0.5f);
		}
		if (isMatrixButtonPressed(buffer, 1, 3)) {
			std::cout << "Matrix button (1,3) is pressed" << std::endl;
			matrixController.setMatrixButtonLED(MatrixButton::matrix13, ColorSystem::lightorange, 0.5f);
		}
		if (isMatrixButtonPressed(buffer, 1, 4)) {
			std::cout << "Matrix button (1,4) is pressed" << std::endl;
			matrixController.setMatrixButtonLED(MatrixButton::matrix14, ColorSystem::warmyellow, 0.5f);
		}
		if (isMatrixButtonPressed(buffer, 2, 1)) {
			std::cout << "Matrix button (2,1) is pressed" << std::endl;
			matrixController.setMatrixButtonLED(MatrixButton::matrix21, ColorSystem::yellow, 0.5f);
		}
		if (isMatrixButtonPressed(buffer, 2, 2)) {
			std::cout << "Matrix button (2,2) is pressed" << std::endl;
			matrixController.setMatrixButtonLED(MatrixButton::matrix22, ColorSystem::lime, 0.5f);
		}
		if (isMatrixButtonPressed(buffer, 2, 3)) {
			std::cout << "Matrix button (2,3) is pressed" << std::endl;
			matrixController.setMatrixButtonLED(MatrixButton::matrix23, ColorSystem::green, 0.5f);
		}
		if (isMatrixButtonPressed(buffer, 2, 4)) {
			std::cout << "Matrix button (2,4) is pressed" << std::endl;
			matrixController.setMatrixButtonLED(MatrixButton::matrix24, ColorSystem::mint, 0.5f);
		}
		if (isMatrixButtonPressed(buffer, 3, 1)) {
			std::cout << "Matrix button (3,1) is pressed" << std::endl;
			matrixController.setMatrixButtonLED(MatrixButton::matrix31, ColorSystem::cyan, 0.5f);
		}
		if (isMatrixButtonPressed(buffer, 3, 2)) {
			std::cout << "Matrix button (3,2) is pressed" << std::endl;
			matrixController.setMatrixButtonLED(MatrixButton::matrix32, ColorSystem::turquise,0.5f);
		}
		if (isMatrixButtonPressed(buffer, 3, 3)) {
			std::cout << "Matrix button (3,3) is pressed" << std::endl;
			matrixController.setMatrixButtonLED(MatrixButton::matrix33, ColorSystem::blue, 0.5f);
		}
		if (isMatrixButtonPressed(buffer, 3, 4)) {
			std::cout << "Matrix button (3,4) is pressed" << std::endl;
			matrixController.setMatrixButtonLED(MatrixButton::matrix34, ColorSystem::plum, 0.5f);
		}
		if (isMatrixButtonPressed(buffer, 4, 1)) {
			std::cout << "Matrix button (4,1) is pressed" << std::endl;
			matrixController.setMatrixButtonLED(MatrixButton::matrix41, ColorSystem::violet, 0.5f);
		}
		if (isMatrixButtonPressed(buffer, 4, 2)) {
			std::cout << "Matrix button (4,2) is pressed" << std::endl;
			matrixController.setMatrixButtonLED(MatrixButton::matrix42, ColorSystem::magenta, 0.5f);
		}
		if (isMatrixButtonPressed(buffer, 4, 3)) {
			std::cout << "Matrix button (4,3) is pressed" << std::endl;
			matrixController.setMatrixButtonLED(MatrixButton::matrix43, ColorSystem::purple, 0.5f);
		}
		if (isMatrixButtonPressed(buffer, 4, 4)) {
			std::cout << "Matrix button (4,4) is pressed" << std::endl;
			matrixController.setMatrixButtonLED(MatrixButton::matrix44, ColorSystem::fuchsia, 0.5f);
		}

		// _________________________________________________________
		// Sleep for a short duration to avoid spamming
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
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