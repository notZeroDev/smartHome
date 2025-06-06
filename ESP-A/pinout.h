#ifndef PINOUT
#define PINOUT

// demand side
extern const int potAC;
extern const int potHeater;
extern const int potLight;

// sensors Handler
extern const int gasSensorPin;

// LED pins
extern const int ledAC;
extern const int ledHeater;
extern const int ledLight;

// RFID (MFRC522 via SPI)
extern const int rfidSS;
extern const int rfidRST;
extern const int spiSCK;
extern const int spiMOSI;
extern const int spiMISO;

// Serial communication with Arduino
extern const int SERIAL_RX;
extern const int SERIAL_TX;

#endif