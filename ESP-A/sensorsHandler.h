#ifndef SENSOR_HANDLERS_H
#define SENSOR_HANDLERS_H

#include <Arduino.h>


// LED States
extern bool led1_state;
extern bool led2_state;
extern bool led3_state;

// Thresholds
extern const int gasThreshold;

// Last known states
extern bool lg;
extern bool lw;
extern bool pot1, pot2, pot3;

// Sensor values
extern int gasValue;

// Function Declarations
void checkGas();

#endif
