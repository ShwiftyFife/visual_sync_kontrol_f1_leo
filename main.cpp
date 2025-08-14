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

// F1 uses 22 bytes for HID reading reports
// F1 uses 81 bytes for HID writing reports

/*
To find hidraw device in bash terminal run:
1.: ls -l /dev/hidraw*
2.: Replace # after hidraw with number of ls list:
	udevadm info -a -n /dev/hidraw# | grep idVendor -A2
3.: Look for: idVendor and idProduct
*/ 

int main() {
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
    
	usleep(2000);  // Small delay

/*


	// Second packet with remaining data  
	unsigned char buf2[10] = {0};  // 18 data bytes + 1 report ID
	buf2[0] = 0x81;  // Same report ID?
	// buf2[1] corresponds to original byte 63
	// buf2[2] corresponds to original byte 64, etc.
	ssize_t written2 = write(fd, buf2, sizeof(buf2));

	if (written2 != sizeof(buf2)) {
        perror("write");
        printf("Only wrote %zd bytes out of %zu\n", written2, sizeof(buf2));
    } else {
        printf("Successfully wrote buf2 %zd bytes\n", written2);
    }
*/
    close(fd);
    return 0;
}