#include "demandSide.h"
#include "sensorsHandler.h"

// LED pins
const int ledAC = 4;
const int ledHeater = 5;
const int ledLight = 6;
void setup() {
  Serial.begin(9600);

  // demand side
  pinMode(ledAC, OUTPUT);
  pinMode(ledHeater, OUTPUT);
  pinMode(ledLight, OUTPUT);
  Serial.println("System started. All devices ON.");
  digitalWrite(ledAC, LOW);
  digitalWrite(ledHeater, LOW);
  digitalWrite(ledLight, LOW);

}

void loop() {
  
  
  // demand side
  checkLoad();
//  digitalWrite(ledAC, pot3);
//  digitalWrite(ledHeater, pot2);
//  digitalWrite(ledLight, pot1);

  // sensor handlres
  checkSensors();
  digitalWrite(ledAC, !(!lg && !lw && pot1)); // reverse the logic for relay(Active LOW)
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
  delay(1000);
}
