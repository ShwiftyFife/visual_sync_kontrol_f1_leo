# visual_sync_kontrol_f1_leo

## Ideas

* Use shift button to toggle preview mode
* Use capture button to program effects on the fly (Shift + capture = Delete -> reset setting)
* Use encoder to change 4x4 matrix bank

## General info

* F1 uses 22 bytes for HID reading reports
* F1 uses 81 bytes for HID writing reports

## Selecting the deive

### hidraw device

To find hidraw device in bash terminal run:

1. ls -l /dev/hidraw*
2. Replace # after hidraw with number of ls list: udevadm info -a -n /dev/hidraw# | grep idVendor -A2
3. Look for: idVendor and idProduct

## Input format

Ordered by the bytes as they are e.g. written into memory upon performing a read() call on the hidraw device:

* 0 – Report ID (= 0x01)
* 1 – Button matrix (top half)
  * Bit 7 – Button (1, 1)
  * Bit 6 — Button (2, 1)
  * Bit 5 – Button (3, 1)
  * Bit 4 – Button (4, 1)
  * Bit 3 – Button (1, 2)
  * Bit 2 – Button (2, 2)
  * Bit 1 – Button (3, 2)
  * Bit 0 – Button (4, 2)
* 2 – Button matrix (bottom half)
  * Bit 7 – Button (1, 3)
  * Bit 6 — Button (2, 3)
  * Bit 5 – Button (3, 3)
  * Bit 4 – Button (4, 3)
  * Bit 3 – Button (1, 4)
  * Bit 2 – Button (2, 4)
  * Bit 1 – Button (3, 4)
  * Bit 0 – Button (4, 4)
* 3 – Special buttons (bottom half)
  * Bit 7 – Shift
  * Bit 6 – Reverse
  * Bit 5 – Type
  * Bit 4 – Size
  * Bit 3 – Browse
  * Bit 2 – Selector wheel button
  * Bit 1 – don't care
  * Bit 0 – don't care
* 4 – Stop buttons, Special buttons (top half)
  * Bit 7 – Stop 1
  * Bit 6 – Stop 2
  * Bit 5 – Stop 3
  * Bit 4 – Stop 4
  * Bit 3 – Sync
  * Bit 2 – Quant
  * Bit 1 – Capture
  * Bit 0 – don't care
* 5 – Selector wheel
* 6 – Knob 1 LSB
* 7 – Knob 1 MSB
* 8 – Knob 2 LSB
* 9 – Knob 2 MSB
* 10 – Knob 3 LSB
* 11 – Knob 3 MSB
* 12 – Knob 4 LSB
* 13 – Knob 4 MSB
* 14 – Fader 1 LSB
* 15 – Fader 1 MSB
* 16 – Fader 2 LSB
* 17 – Fader 2 MSB
* 18 – Fader 3 LSB
* 19 – Fader 3 MSB
* 20 – Fader 4 LSB
* 21 – Fader 4 MSB

## Output format

Ordered by the bytes as they are e.g. written into memory before performing a `write()` call on the `hidraw` device or `hid_write()` on a `hid_open()` device.

* 0 – Report ID (= 0x80)
* 1 to 8 – 7 segment display, right number
  * 1 – dot
  * 2 – middle
  * 3 – lower right
  * 4 – upper right
  * 5 – top
  * 6 – upper left
  * 7 – lower left
  * 8 – bottom
* 9 to 16 – 7 segment display, left number
  * 9 – dot
  * 10 – middle
  * 11 – lower right
  * 12 – upper right
  * 13 – top
  * 14 – upper left
  * 15 – lower left
  * 16 – bottom
* 17 to 25 – special buttons
  * 17 – Browse
  * 18 – Size
  * 19 – Type
  * 20 – Reverse
  * 21 – Shift
  * 22 – Capture
  * 23 – Quant
  * 24 – Sync
* 25 to 72 – RGB LED button matrix
  * 25 – (1, 1) blue
  * 26 – (1, 1) red
  * 27 – (1, 1) green
  * 28 – (1, 2) blue
  * 29 – (1, 2) red
  * 30 – (1, 2) green
  * 31 – (1, 3) blue
  * 32 – (1, 3) red
  * 33 – (1, 3) green
  * 34 – (1, 4) blue
  * 35 – (1, 4) red
  * 36 – (1, 4) green
  * 37 – (2, 1) blue
  * 38 – (2, 1) red
  * 39 – (2, 1) green
  * 40 – (2, 2) blue
  * 41 – (2, 2) red
  * 42 – (2, 2) green
  * 43 – (2, 3) blue
  * 44 – (2, 3) red
  * 45 – (2, 3) green
  * 46 – (2, 4) blue
  * 47 – (2, 4) red
  * 48 – (2, 4) green
  * 49 – (3, 1) blue
  * 50 – (3, 1) red
  * 51 – (3, 1) green
  * 52 – (3, 2) blue
  * 53 – (3, 2) red
  * 54 – (3, 2) green
  * 55 – (3, 3) blue
  * 56 – (3, 3) red
  * 57 – (3, 3) green
  * 58 – (3, 4) blue
  * 59 – (3, 4) red
  * 60 – (3, 4) green
  * 61 – (4, 1) blue
  * 62 – (4, 1) red
  * 63 – (4, 1) green
  * 64 – (4, 2) blue
  * 65 – (4, 2) red
  * 66 – (4, 2) green
  * 67 – (4, 3) blue
  * 68 – (4, 3) red
  * 69 – (4, 3) green
  * 70 – (4, 4) blue
  * 71 – (4, 4) red
  * 72 – (4, 4) green
* 73 to 80 – Stop buttons (each button has 2 separate LEDs)
  * 73 Stop 4 right
  * 74 Stop 4 left
  * 75 Stop 3 right
  * 76 Stop 3 left
  * 77 Stop 2 right
  * 78 Stop 2 left
  * 79 Stop 1 right
  * 80 Stop 1 left
