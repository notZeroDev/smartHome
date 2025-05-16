#include "pinout.h"
#include <Arduino.h>

#include "pinout.h"
#include <Arduino.h>

// Potentiometer pins (ADC input)
const int potAC = 34;       // Input only, safe ADC
const int potHeater = 35;   // Input only, safe ADC
const int potLight = 32;    // ADC safe

// Sensors Handler
const int gasSensorPin = 33;   
// const int waterSensorPin = 25;  // not working and will be transfared to ESP-b

// LED pins (digital output)
const int ledAC = 26;
const int ledHeater = 27;
const int ledLight = 14;

// Shift register pins (digital output)
// const int dataPin = 13;   // safe digital output
// const int clockPin = 4;   // safe digital output
// const int latchPin = 5;   // safe digital output
