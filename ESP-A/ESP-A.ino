#include "demandSide.h"
#include "sensorsHandler.h"
// #include "shiftRegistor.h"
#include "pinout.h"
#include "../public/env.h"
#include <BlynkSimpleEsp32.h>
#include <WiFiClientSecure.h>
#include "../public/telegram.ino"




void setup() {
  Serial.begin(115200);
  

  // blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  setupTelegramBot();
  sendTelegramMessage("testing from ESP32");
  makeTelegramCall("test call");

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
  // blynk
  Blynk.run();

  // demand side
  checkLoad();
  updatVPin();
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
void updatVPin(){
  int acVal = getACVal(), HeaterVal = getHeaterVal(), lightVal = getLightVal();
  // Blynk.virtualWrite(V10,acVal);
  // Blynk.virtualWrite(V11,HeaterVal);
  // Blynk.virtualWrite(V12,lightVal);
  // Blynk.virtualWrite(V0, acVal + HeaterVal + lightVal);
}
