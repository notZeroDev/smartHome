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

// RFID (MFRC522 via SPI)
const int rfidSS = 5;
const int rfidRST = 22;
const int spiSCK = 18;  
const int spiMOSI = 23;
const int spiMISO = 19;

// LED pins (digital output)
const int ledAC = 26;
const int ledHeater = 27;
const int ledLight = 14;

// Shift register pins (digital output)
// const int dataPin = 13;   // safe digital output
// const int clockPin = 4;   // safe digital output
// const int latchPin = 5;   // safe digital output
