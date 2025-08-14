#include <iostream>			// For standard input output operations
#include <stdio.h> 			// stdio.h gives printf() — basic text output (like print() in R).
#include <wchar.h> 			// wchar.h gives wide-char support (wchar_t) used because USB string descriptors are UTF-16/wide.
#include <unistd.h> 		// unistd.h gives access to the POSIX operating system API.
#include <stdint.h> 		// stdint.h gives uint8_t support. uint8_t is an unsigned 8-bit integer type.
#include <hidapi/hidapi.h>
#include <fcntl.h>
#include <cstring>

// build with control+shift+b (builds with cmake, declared in tasks.json)
// run in terminal with ./main



/*
To find hidraw device in bash terminal run:
1.: ls -l /dev/hidraw*
2.: Replace # after hidraw with number of ls list:
	udevadm info -a -n /dev/hidraw# | grep idVendor -A2
3.: Look for: idVendor and idProduct
*/ 


void toggleLED(hid_device *handle) {
    // Green LED positions for the 4x4 matrix (reading left to right, top to bottom)
    int green_positions[16] = {
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

    unsigned char buf[81] = {0};
    int buf_len = sizeof(buf) / sizeof(buf[0]);
    buf[0] = 0x80; // Set report ID
    
    // Turn on each green LED sequentially
    for (int i = 0; i < 16; i++) {
        // Clear previous LED (turn off all LEDs)
        memset(&buf[1], 0, 80); // Clear bytes 1-80, keep report ID
        
        // Turn on current green LED
        buf[green_positions[i]] = 127; // Full brightness
        
        // Send the command
        int res = hid_write(handle, buf, buf_len);
        if (res < 0) {
            printf("Error writing to device: %ls\n", hid_error(handle));
            return;
        }
        
        // Wait before moving to next LED
        usleep(500000); // Convert ms to microseconds
    }
    
    // Optional: Turn off all LEDs at the end
    memset(&buf[1], 0, 80);
    hid_write(handle, buf, buf_len);
}


int main() {

    // create start up message
	printf("Starting Visual Sync Kontrol F1 hello Leo\n");
	
	// Initialize HIDAPI return code, send error message if initialization fails
	int res = hid_init();

	if (res != 0) {
		fprintf(stderr, "Failed to initialize HIDAPI\n");
		return 1;
	} else {
		printf("HID_API initialized successfully!\n");
	}

    hid_device *handle; // HID device handle
    // Open the device using the VendorID, ProductID, and optionally the Serial number.
	handle = hid_open(0x17cc, 0x1120, NULL);
	if (handle) {
		toggleLED(handle);
		printf("Traktor Kontrol F1 opened successfully!\n");
		// Proceed with other operations
	} else {
		printf("Unable to open device\n");
		hid_exit();
 		return 0;
	}

    // Close the device
	hid_close(handle);

	// Finalize the hidapi library
	res = hid_exit();

    return 0;
}