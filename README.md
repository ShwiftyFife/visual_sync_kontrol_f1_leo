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
  * 28 – (2, 1) blue
  * 29 – (2, 1) red
  * 30 – (2, 1) green
  * 31 – (3, 1) blue
  * 32 – (3, 1) red
  * 33 – (3, 1) green
  * 34 – (4, 1) blue
  * 35 – (4, 1) red
  * 36 – (4, 1) green
  * 37 – (1, 2) blue
  * 38 – (1, 2) red
  * 39 – (1, 2) green
  * 40 – (2, 2) blue
  * 41 – (2, 2) red
  * 42 – (2, 2) green
  * 43 – (3, 2) blue
  * 44 – (3, 2) red
  * 45 – (3, 2) green
  * 46 – (4, 2) blue
  * 47 – (4, 2) red
  * 48 – (4, 2) green
  * 49 – (1, 3) blue
  * 50 – (1, 3) red
  * 51 – (1, 3) green
  * 52 – (2, 3) blue
  * 53 – (2, 3) red
  * 54 – (2, 3) green
  * 55 – (3, 3) blue
  * 56 – (3, 3) red
  * 57 – (3, 3) green
  * 58 – (4, 3) blue
  * 59 – (4, 3) red
  * 60 – (4, 3) green
  * 61 – (1, 4) blue
  * 62 – (1, 4) red
  * 63 – (1, 4) green
  * 64 – (2, 4) blue
  * 65 – (2, 4) red
  * 66 – (2, 4) green
  * 67 – (3, 4) blue
  * 68 – (3, 4) red
  * 69 – (3, 4) green
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
