#include "demandSide.h"
#include "sensorsHandler.h"
// #include "shiftRegistor.h"
#include "pinout.h"
#include "../public/env.h"
#include <BlynkSimpleEsp32.h>
#include <WiFiClientSecure.h>
#include "../public/telegram.ino"

void checkSerial();
String formatDigit(String);

void setup() {
  Serial.begin(115200);
  
  
  // blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  // Serial communcation with esp32
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // RX=GPIO16, TX=GPIO17
  
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
  checkSerial();


  // blynk
  Blynk.run();

  // demand side
  checkLoad();
  updatVPin();
  //  digitalWrite(ledAC, pot3);
  //  digitalWrite(ledHeater, pot2);
  //  digitalWrite(ledLight, pot1);

  // sensor handlres
  checkGas();
  // update Loads
  digitalWrite(ledAC, !(!lg && !lw && pot1));  // reverse the logic for relay(Active LOW)
  digitalWrite(ledHeater, !(!lg && pot2));
  digitalWrite(ledLight, !(!lg && pot3));

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
void checkSerial(){
  if (Serial2.available()) {
    String incoming = Serial2.readStringUntil('\n');
    incoming.trim();
    if (incoming == "WATER HIGH"){
    lw = true;
    makeTelegramCall("water leakage on your house");
    } 
    else if (incoming == "WATER LOW") lw = false;
  }
}
String formatDigit(String input){
  String output = "";
  int len =input.length();

  for (int i = 0; i < len; i++) {
    output +=input[i];
    // Add dash after every 4 digits except the last group
    if ((i + 1) % 4 == 0 && (i + 1) < len) {
      output += '-';
    }
  }

  return output;
}
BLYNK_WRITE(V1) {
    int value = param.asInt();
    if (value == 1) {
    randomSeed(analogRead(0)); 
    String otp = String(random(10000000, 100000000));
    String formatedOTP = formatDigit(otp);
    //! send it via serial
    Serial2.print("OTP: ");
    Serial2.println(otp);
    Blynk.virtualWrite (V1,formatedOTP);
    sendTelegramMessage(formatedOTP);
    }
  }