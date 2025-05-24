#include "pinout.h"
#include <Arduino.h>

#include "pinout.h"
#include <Arduino.h>

// Potentiometer pins (ADC input)
const int potAC = 35;
const int potHeater = 34;
const int potLight = 32;

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

// Serial communication with Arduino
const int SERIAL_RX = 16;
const int SERIAL_TX = 17;
