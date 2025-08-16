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

// Include startup effects module
#include "headers/startup_effect.h"
// Include color system module
#include "headers/color_system.h"
// Include matrix LED module
#include "headers/set_matrix_led.h"


int main() {

	// ________________________________________________________________
	// START UP SEQUENCE

	// create start up message
	printf("Starting Visual Sync Kontrol F1\n");
	
	// Initialize HIDAPI return code, send error message if initialization fails
	int res = hid_init();
	if (res != 0) {
		printf("- Failed to initialize HIDAPI\n");
		return 1;
	} else {
		printf("- HID_API initialized successfully!\n");
	}

	// Open the device:
	// Declare pointer to HID device handle
	hid_device *handle; 
	
	// Open the device using the VendorID, ProductID, and optionally the Serial number.
	handle = hid_open(0x17cc, 0x1120, NULL);
	if (handle) {
		printf("- Opening Traktor Kontrol F1...\n");
		startupLEDSequence(handle);
		printf("- Traktor Kontrol F1 opened successfully!\n");
	} else {
		printf("- Unable to open device...\nShutting down...\n");
		hid_exit();
        return 0;
	}
	// ________________________________________________________________
	// STUFFs HAPPENIN HERE
	MatrixLEDController matrixController(handle);

	matrixController.setMatrixButtonLED(MatrixButton::matrix11, ColorSystem::red, 0.1f);
	matrixController.setMatrixButtonLED(MatrixButton::matrix12, ColorSystem::red, 0.1f);
	matrixController.setMatrixButtonLED(MatrixButton::matrix13, ColorSystem::red, 0.1f);
	matrixController.setMatrixButtonLED(MatrixButton::matrix14, ColorSystem::red, 0.1f);

	matrixController.setMatrixButtonLED(MatrixButton::matrix21, ColorSystem::yellow, 0.1f);
	matrixController.setMatrixButtonLED(MatrixButton::matrix22, ColorSystem::yellow, 0.1f);
	matrixController.setMatrixButtonLED(MatrixButton::matrix23, ColorSystem::yellow, 0.1f);
	matrixController.setMatrixButtonLED(MatrixButton::matrix24, ColorSystem::yellow, 0.1f);

	matrixController.setMatrixButtonLED(MatrixButton::matrix31, ColorSystem::cyan, 0.1f);
	matrixController.setMatrixButtonLED(MatrixButton::matrix32, ColorSystem::cyan, 0.1f);
	matrixController.setMatrixButtonLED(MatrixButton::matrix33, ColorSystem::cyan, 0.1f);
	matrixController.setMatrixButtonLED(MatrixButton::matrix34, ColorSystem::cyan, 0.1f);

	matrixController.setMatrixButtonLED(MatrixButton::matrix41, ColorSystem::violet, 0.1f);
	matrixController.setMatrixButtonLED(MatrixButton::matrix42, ColorSystem::violet, 0.1f);
	matrixController.setMatrixButtonLED(MatrixButton::matrix43, ColorSystem::violet, 0.1f);
	matrixController.setMatrixButtonLED(MatrixButton::matrix44, ColorSystem::violet, 0.1f);
	
	// ________________________________________________________________
	// CODE CLOSES
  // Close the device
	hid_close(handle);

	// Finalize the hidapi library
	res = hid_exit();



	// ________________________________________________________________
	// TEST

  return 0;
}