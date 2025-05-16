#include "demandSide.h"
#include "sensorsHandler.h"
// #include "shiftRegistor.h"
#include "pinout.h"
#include "../public/env.h"
void setup() {
  Serial.begin(115200);

  // demand side
  pinMode(ledAC, OUTPUT);
  pinMode(ledHeater, OUTPUT);
  pinMode(ledLight, OUTPUT);
  Serial.println("System started. All devices ON.");
  digitalWrite(ledAC, LOW);
  digitalWrite(ledHeater, LOW);
  digitalWrite(ledLight, LOW);


  // sensors handler
  pinMode(gasSensorPin, INPUT);
  pinMode(waterSensorPin, INPUT);

  // shift registor
  // pinMode(dataPin, OUTPUT);
  // pinMode(clockPin, OUTPUT);
  // pinMode(latchPin, OUTPUT);
  // sendShiftRegisterOutput();
}

void loop() {


  // demand side
  checkLoad();
  //  digitalWrite(ledAC, pot3);
  //  digitalWrite(ledHeater, pot2);
  //  digitalWrite(ledLight, pot1);

  // sensor handlres
  checkSensors();
  digitalWrite(ledAC, !(!lg && !lw && pot1));  // reverse the logic for relay(Active LOW)
  digitalWrite(ledHeater, !(!lg && pot2));
  digitalWrite(ledLight, !(!lg && pot3));
  Serial.print("lg: ");
  Serial.println(lg);
  Serial.print("lw: ");
  Serial.println(lw);
  Serial.print("pot1: ");
  Serial.println(pot1);
  Serial.print("pot2: ");
  Serial.println(pot2);
  Serial.print("pot3: ");
  Serial.println(pot3);

  // shift registor
  // setOutputPin(0, 1); // example
  delay(1000);
}
