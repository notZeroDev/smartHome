#include "env.h"
#include <BlynkSimpleEsp32.h>
#include "blynkAPI.h"
#include "demandSide.h"
#include "telegram.h"

// Helper function
String formatDigit(String input) {
  String output = "";
  int len = input.length();

  for (int i = 0; i < len; i++) {
    output += input[i];
    // Add dash after every 4 digits except the last group
    if ((i + 1) % 4 == 0 && (i + 1) < len) {
      output += '-';
    }
  }
  return output;
}

void updateVirtualPin(int pin, auto data) {
  Blynk.virtualWrite(pin, data);
}
void blynkSetup() {
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Blynk.virtualWrite(V16, 0);
}
void blynkRun() {
  Blynk.run();
}
void updateACLoad(bool state) {
  Blynk.virtualWrite(V2, state);  // AC Load
}
void updateHeaterLoad(bool state) {
  Blynk.virtualWrite(V7, state);  // Heater Load
}
void updateLightLoad(bool state) {
  Blynk.virtualWrite(V8, state);  // Light Load
}
void updateLoadsVirtualPins(bool ACLoad, bool heaterLoad, bool lightLoad) {
  updateACLoad(ACLoad);
  updateHeaterLoad(heaterLoad);
  updateLightLoad(lightLoad);
}
void updateLoadsStates() {
  int acVal = getACVal(), HeaterVal = getHeaterVal(), lightVal = getLightVal();
  Blynk.virtualWrite(V10, (pot3 * acVal * 0.007324219));
  Blynk.virtualWrite(V11, (pot2 * HeaterVal * 0.007324219));
  Blynk.virtualWrite(V12, (pot1 * lightVal * 0.007324219));
  Blynk.virtualWrite(V0, (acVal * pot3 + HeaterVal * pot2 + lightVal * pot1) * 0.007324219);
}

BLYNK_WRITE(V1) {
  Serial.println("New OTP Generated");
  int value = param.asInt();
  if (value == 1) {
    randomSeed(analogRead(0));
    String otp = String(random(10000000, 100000000));
    String formatedOTP = formatDigit(otp);
    Serial2.print("0");
    Serial2.println(otp);
    Blynk.virtualWrite(V1, formatedOTP);
    sendTelegramMessage(String("OTP generated: ") + formatedOTP);
  }
}
BLYNK_WRITE(V16) {  // NFC
  nfcEnabled = param.asInt();
  if (nfcEnabled) Serial2.print("2");  // NFC Enabled
  else Serial2.print("3");             // NFC Disabled
}
BLYNK_WRITE(V18) {  // Locked
  int value = param.asInt();
  if (value == 1) {
    Serial2.print("9");
  }
}
BLYNK_WRITE(V13) {  // enable messages
  enableMessage = param.asInt();
}
BLYNK_WRITE(V14) {  // enable calls
  enableCalls = param.asInt();
}
BLYNK_WRITE(V19) {  // enable logs
  enableLogs = param.asInt();
}
BLYNK_WRITE(V2) {  // AC Load Active
  pot3 = param.asInt();
}
BLYNK_WRITE(V7) {  // Heater Load Active
  pot2 = param.asInt();
}
BLYNK_WRITE(V8) {  // Light Load Active
  pot1 = param.asInt();
}