#ifndef STARTUP_EFFECT_H
#define STARTUP_EFFECT_H

#include <hidapi/hidapi.h>
#include <stdint.h>

void clearAllLEDs(hid_device *handle);
void startupLEDSequence(hid_device *handle);

#endif