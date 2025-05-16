#ifndef SENSOR_HANDLERS_H
#define SENSOR_HANDLERS_H

#include <Arduino.h>


// LED States
extern bool led1_state;
extern bool led2_state;
extern bool led3_state;

// Thresholds
extern const int gasThreshold;
extern const int waterThreshold;

// Last known states
extern bool lg;
extern bool lw;

// Sensor values
extern int gasValue;
extern int waterValue;

// Function Declarations
void checkWater();
void checkGas();
void checkSensors();

#endif
