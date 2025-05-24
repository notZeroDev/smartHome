#ifndef PINOUT_H
#define PINOUT_H
#include <Arduino.h>

#define Rows 4
#define Cols 3

extern byte rowPins[Rows];
extern byte colPins[Cols];

#define SERVO_PIN 12
#define BuzzerPin 13
#define WATER_PIN A0
#define SERIAL_RX 2
#define SERIAL_TX 3

#endif
