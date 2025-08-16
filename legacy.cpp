/*
void toggleLED(hid_device *handle) {
    unsigned char buf[62] = {0};

	int buf_len = sizeof(buf) / sizeof(buf[0]);

    buf[0] = 0x80; // Set report ID to toggle LED

	int i;
	for (i = 1; i < buf_len; i++) {
		buf[i] = 0x00;
	}

	int res = hid_write(handle, buf, buf_len);


	//buf[buf_len - 1] = 0x7f;

	//int i;
	//for (i = 1; i < buf_len; i++) {
	//	buf[i] = 0x7f;
	//}


    //int res = hid_write(handle, buf, buf_len);
	

    if (res < 0) {
        fprintf(stderr, " - Failed to send LED toggle command\n");
    } else {
		// Print out the returned buffer.
		int i;
		for (i = 0; i < buf_len; i++) {
			printf("buf[%d]: %d\n", i, buf[i]);
		}

		printf(" - LED toggle command sent successfully\n");
    }
}
*/

//int main(int argc, char* argv[]) {

	/*

    (void)argc;
    (void)argv;

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

	//wchar_t wstr[MAX_STR]; // Wide string buffer
	//hid_device *handle; // HID device handle

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

	
	// THIS WORKS!

    //wchar_t wstr[MAX_STR]; // Wide string buffer

	// Read the Manufacturer String
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	printf("Manufacturer String: %ls\n", wstr);

	// Read the Product String
	res = hid_get_product_string(handle, wstr, MAX_STR);
	printf("Product String: %ls\n", wstr);

	// Read the Serial Number String
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	printf("Serial Number String: (%d) %ls\n", wstr[0], wstr);

	// Read Indexed String 1
	int i;
	while (true) {
		unsigned char buf[22];
		// Read requested state
		res = hid_read(handle, buf, 22);

		// Print out the returned buffer.
		for (i = 0; i < 22; i++) {
			printf("buf[%d]: %d\n", i, buf[i]);
		}
	}
	

	// Close the device
	hid_close(handle);

	// Finalize the hidapi library
	res = hid_exit();

	*/
















	/*
#define MAX_STR 255

int toggleLED(int device) {

	uint8_t buf[64] = {0};

	buf[0] = 0x80;	// Set report ID to toggle LED
	buf[51] = 0x7f;

	ssize_t res = write(device, buf, sizeof(buf));

	if (res < 0) {
        perror("write failed");
		return -1;
    } else if (res != sizeof(buf)) {
        fprintf(stderr, "Short write: %zd of %zu bytes\n", res, sizeof(buf));
		return -1;
    } else {
        printf("LED packet sent successfully (%zu bytes)\n", sizeof(buf));
    }

	return 0;
}





int main() {

	int device = open("/dev/hidraw3", O_RDWR | O_NONBLOCK); // Open the hidraw device file

	if (device < 0) {
		perror("Failed to open hidraw device");
		return -1;
	} else {
		printf("HID raw device opened successfully!\n");
	}

	// Toggle LED
	if (toggleLED(device) < 0) {
		close(device);
		return -1;
	}

	close(device); // Close the hidraw device file
	printf("HID raw device closed successfully!\n");

	// exit program
    return 0;
}

*/














/*
    int fd = open("/dev/hidraw9", O_RDWR);  // adjust device number
    if (fd < 0) {
        perror("not open");
        return 1;
    }
    
    // Create 81-byte report (same as reference code)
    unsigned char buf1[81] = {0};
    buf1[0] = 0x80;           // Report ID
	buf1[25] = 0;
	buf1[26] = 0;
	buf1[27] = 127;
    buf1[71] = 127;


    // Write the full report
    ssize_t written1 = write(fd, buf1, sizeof(buf1));
    if (written1 != sizeof(buf1)) {
        perror("write");
        printf("Only wrote %zd bytes out of %zu\n", written1, sizeof(buf1));
    } else {
        printf("Successfully wrote buf1 %zd bytes\n", written1);
    }
    
//	usleep(2000);  // Small delay

    close(fd);
*/


















/*

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
        usleep(100000); // Convert ms to microseconds
    }
    
    // Optional: Turn off all LEDs at the end
    memset(&buf[1], 0, 80);
    hid_write(handle, buf, buf_len);
}

*/


