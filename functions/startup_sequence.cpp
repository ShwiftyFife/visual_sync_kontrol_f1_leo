#include "headers/startup_sequence.h"

#include <iostream>      // iostream gives std::cout
#include <unistd.h>      // unistd.h gives usleep() — sleep for microseconds
#include <cstring>       // cstring gives memset() — memory manipulation
// #include <hidapi/hidapi.h>   // included already in header


// =============================================================================
// START UP SEQUENCE
// =============================================================================

/*
* Run the startup sequence for the device
* 
* @param handle: The HID device handle
*/

void startupSequence(hid_device *handle) {
  
  // Step 1: Check if handle is valid
  if (!handle) {
      std::cout << "Error: Invalid device handle for startup sequence" << std::endl;
      return;
  }

  // Define sleep time
  const int sleep_time = 50000;

  // Run startup sequence message
  std::cout << "  - Running startup LED sequence..." << std::endl;

  // ________________________________
  // Led sequence
  // 1
  unsigned char buf[81] = {0};
  buf[0] = 0x80; // Set report ID
  buf[63] = 64; // Turn on (1,4) green LED
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time); // Wait before moving to next LED (100ms)

  // 2
  memset(&buf[1], 0, 80); // Clear bytes 1-80, keep report ID
  buf[63] = 127; // Turn on (1,4) green LED
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time);

  // 3
  memset(&buf[1], 0, 80);
  buf[63] = 127; // Turn on (1,4) green LED
  buf[51] = 64; // Turn on (1,3) green LED
  buf[66] = 64; // Turn on (2,4) green LED
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time);

  // 4
  memset(&buf[1], 0, 80);
  buf[63] = 127; // Turn on (1,4) green LED
  buf[51] = 127; // Turn on (1,3) green LED
  buf[66] = 127; // Turn on (2,4) green LED
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time);

  // 5
  memset(&buf[1], 0, 80);
  buf[63] = 64; // Turn on (1,4) green LED
  buf[51] = 127; // Turn on (1,3) green LED
  buf[66] = 127; // Turn on (2,4) green LED
  buf[39] = 64; // Turn on (1,2) green LED
  buf[54] = 64; // Turn on (2,3) green LED
  buf[69] = 64; // Turn on (3,4) green LED
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time);

  // 6
  memset(&buf[1], 0, 80);
  buf[51] = 127; // Turn on (1,3) green LED
  buf[66] = 127; // Turn on (2,4) green LED
  buf[39] = 127; // Turn on (1,2) green LED
  buf[54] = 127; // Turn on (2,3) green LED
  buf[69] = 127; // Turn on (3,4) green LED
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time);

  // 7
  memset(&buf[1], 0, 80);
  buf[51] = 64; // Turn on (1,3) green LED
  buf[66] = 64; // Turn on (2,4) green LED
  buf[39] = 127; // Turn on (1,2) green LED
  buf[54] = 127; // Turn on (2,3) green LED
  buf[69] = 127; // Turn on (3,4) green LED
  buf[27] = 64; // Turn on (1,1) green LED
  buf[42] = 64; // Turn on (2,2) green LED
  buf[57] = 64; // Turn on (3,3) green LED
  buf[72] = 64; // Turn on (4,4) green LED
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time);

  // 8
  memset(&buf[1], 0, 80);
  buf[39] = 127; // Turn on (1,2) green LED
  buf[54] = 127; // Turn on (2,3) green LED
  buf[69] = 127; // Turn on (3,4) green LED
  buf[27] = 127; // Turn on (1,1) green LED
  buf[42] = 127; // Turn on (2,2) green LED
  buf[57] = 127; // Turn on (3,3) green LED
  buf[72] = 127; // Turn on (4,4) green LED
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time);

  // 9
  memset(&buf[1], 0, 80);
  buf[39] = 64; // Turn on (1,2) green LED
  buf[54] = 64; // Turn on (2,3) green LED
  buf[69] = 64; // Turn on (3,4) green LED
  buf[27] = 127; // Turn on (1,1) green LED
  buf[42] = 127; // Turn on (2,2) green LED
  buf[57] = 127; // Turn on (3,3) green LED
  buf[72] = 127; // Turn on (4,4) green LED
  buf[30] = 64; // Turn on (1,4) green LED
  buf[45] = 64; // Turn on (2,4) green LED
  buf[60] = 64; // Turn on (3,4) green LED
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time);

  // 10
  memset(&buf[1], 0, 80);
  buf[27] = 127; // Turn on (1,1) green LED
  buf[42] = 127; // Turn on (2,2) green LED
  buf[57] = 127; // Turn on (3,3) green LED
  buf[72] = 127; // Turn on (4,4) green LED
  buf[30] = 127; // Turn on (1,4) green LED
  buf[45] = 127; // Turn on (2,4) green LED
  buf[60] = 127; // Turn on (3,4) green LED
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time);

  // 11
  memset(&buf[1], 0, 80);
  buf[27] = 64; // Turn on (1,1) green LED
  buf[42] = 64; // Turn on (2,2) green LED
  buf[57] = 64; // Turn on (3,3) green LED
  buf[72] = 64; // Turn on (4,4) green LED
  buf[30] = 127; // Turn on (1,4) green LED
  buf[45] = 127; // Turn on (2,4) green LED
  buf[60] = 127; // Turn on (3,4) green LED
  buf[33] = 64; // Turn on (1,2) green LED
  buf[48] = 64; // Turn on (2,3) green LED
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time);

  // 12
  memset(&buf[1], 0, 80);
  buf[30] = 127;
  buf[45] = 127;
  buf[60] = 127;
  buf[33] = 127;
  buf[48] = 127;
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time);

  // Step 13
  memset(&buf[1], 0, 80);
  buf[30] = 64; // Turn on (1,4) green LED
  buf[45] = 64; // Turn on (2,4) green LED
  buf[60] = 64; // Turn on (3,4) green LED
  buf[33] = 127; // Turn on (1,2) green LED
  buf[48] = 127; // Turn on (2,3) green LED
  buf[36] = 64; // Turn on (1,3) green LED
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time);

  // Step 14
  memset(&buf[1], 0, 80);
  buf[33] = 127; // Turn on (1,2) green LED
  buf[48] = 127; // Turn on (2,3) green LED
  buf[36] = 127; // Turn on (1,3) green LED
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time);

  // Step 15
  memset(&buf[1], 0, 80);
  buf[33] = 64; // Turn on (1,2) green LED
  buf[48] = 64; // Turn on (2,3) green LED
  buf[36] = 127; // Turn on (1,3) green LED
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time);

  // Step 16
  memset(&buf[1], 0, 80);
  buf[36] = 127; // Turn on (1,3) green LED
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time);

  // Step 17
  memset(&buf[1], 0, 80);
  buf[36] = 64;
  hid_write(handle, buf, sizeof(buf));
  usleep(sleep_time);

  // Clear all leds
  memset(&buf[1], 0, 80);
  //buf[21] = 64; // Turn on shift LED
  //buf[17] = 64; // Turn on browse LED
  hid_write(handle, buf, sizeof(buf));

  std::cout << "  - Startup Sequence completed!" << std::endl;
}