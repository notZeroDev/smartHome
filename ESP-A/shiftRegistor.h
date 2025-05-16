#ifndef SHIFTREGISTER_H
#define SHIFTREGISTER_H

// Output state for all 16 pins (2 x 8-bit shift registers)
extern bool outputStates[16];  // Initialized to LOW



void setOutputPin(int, bool);
void sendShiftRegisterOutput();
#endif
