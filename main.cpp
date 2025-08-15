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

// Include our startup effects module
#include "headers/startup_effect.h"


int main() {


    // ________________________________________________________________
    // START UP SEQUENCE

    // create start up message
	printf("Starting Visual Sync Kontrol F1\n");
	
	// Initialize HIDAPI return code, send error message if initialization fails
	int res = hid_init();
	if (res != 0) {
		fprintf(stderr, "Failed to initialize HIDAPI\n");
		return 1;
	} else {
		printf("HID_API initialized successfully!\n");
	}

    // Open the device:
    // Declare pointer to HID device handle
    hid_device *handle; 
    
    // Open the device using the VendorID, ProductID, and optionally the Serial number.
	handle = hid_open(0x17cc, 0x1120, NULL);
	if (handle) {
		startupLEDSequence(handle);
		printf("Traktor Kontrol F1 opened successfully!\n");
	} else {
		printf("Unable to open device...\nShutting down...\n");
		hid_exit();
        return 0;
	}

    // Close the device
	hid_close(handle);

	// Finalize the hidapi library
	res = hid_exit();

    return 0;
}