#ifndef PINOUT_H
#define PINOUT_H
#include <Arduino.h>

#define Rows 4
#define Cols 4

extern byte rowPins[Rows];
extern byte colPins[Cols];

#define SERVO_PIN 12
#define BuzzerPin 13
#define WATER_PIN A0


#endif
