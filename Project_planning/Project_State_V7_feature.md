# Traktor F1 Fader and knob input reader

## Description

### Knobs

Knobs provide 12 bits of precision and are stored LSB first throughout bytes 6 to 13 with each knob being represented by 2 bytes (the upper 4 bits of MSB are all zero). Turning the knob to the leftmost position gives the minimum value (near 0x000) while turning the knob to the rightmost position gives the maximum value (near 0xFFF). As the values are not exactly 0x000 or 0xFFF, calibration might be necessary to determine the leftmost, rightmost and therefore middle position of the knob.

**Example calibration of the testing device:**

Knob 1: 0x001 to 0xFFB
Knob 2: 0x002 to 0xFFB
Knob 3: 0x000 to 0xFFA
Knob 4: 0x001 to 0xFFC

The exact calibration has fluctuations of about 2, but is apparently always nearly from 0x000 to 0xFFF (at least for the testing device).

### Faders

Faders provide 12 bits of precision anologous to knobs and are stored LSB first throughout bytes 14 to 21. Faders range from 0x000 (bottom position) to 0xFFF (top position, value assumed) and need to be calibrated like the knobs.

**Example calibration of the testing devices:**

Fader 1: 0x000 to 0xFF6
Fader 2: 0x001 to 0xFF8
Fader 3: 0x000 to 0xFF7
Fader 4: 0x000 to 0xFF7

Like the knobs, the exact calibration has fluctuations of about 2, but is apparently always somewhere near 0x000 to 0xFFF (at least for the testing device).

## Suggested files

### headers

- headers/input_reader_fader.h
- headers/input_reader_knob.h

### sources

- functions/input_reader_fader.cpp
- functions/input_reader_knob.cpp
