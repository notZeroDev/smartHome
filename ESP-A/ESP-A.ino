#include "demandSide.h"
#include "sensorsHandler.h"
// #include "shiftRegistor.h"
#include "pinout.h"
#include "env.h"
#include <BlynkSimpleEsp32.h>
#include <WiFiClientSecure.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
//#include <MFRC522DriverI2C.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h> 
#include "telegram.h"
#include <map>
#include <string>

std::map<String, String> uidToName = {
  {"40625b1b", "Mohamed"},
  {"819b4b27", "Marwan"},
};
void checkSerial();
String formatDigit(String);
bool cardPresent = false;  // Make sure this is global!

// Create the ss_pin object before driver
MFRC522DriverPinSimple ss_pin(rfidSS);

// Create the SPI driver instance (note the correct constructor params)
// The SPIClass instance is usually "SPI" for default SPI on ESP32.
// The SPISettings are optional, defaults are usually fine.
MFRC522DriverSPI driver(ss_pin, SPI);  

MFRC522 mfrc522(driver);
void setup() {
  Serial.begin(115200);
  Serial.println("HELLO FROM SERIAL");

  SPI.begin(spiSCK, spiMISO, spiMOSI);
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

  // shift registor
  // pinMode(dataPin, OUTPUT);
  // pinMode(clockPin, OUTPUT);
  // pinMode(latchPin, OUTPUT);
  // sendShiftRegisterOutput();
  mfrc522.PCD_Init();    // Init MFRC522 board.
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);
  Serial.println(F("Scan PICC to see UID"));
}

void loop() {
  checkSerial();


  // blynk
  Blynk.run();

  // demand side
  checkLoad();
  updatVPin();
   digitalWrite(ledAC, pot3);
   digitalWrite(ledHeater, pot2);
   digitalWrite(ledLight, pot1);

  // sensor handlres
  checkGas();
  // update Loads
  digitalWrite(ledAC, !(!lg && !lw && pot1));  // reverse the logic for relay(Active LOW)
  digitalWrite(ledHeater, !(!lg && pot2));
  digitalWrite(ledLight, !(!lg && pot3));

  // shift registor
  // setOutputPin(0, 1); // example
  // If a new card is present and not already marked as present
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    if (!cardPresent) {
      cardPresent = true;  // Mark card as present

      // Save UID as string
      String uidString = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (mfrc522.uid.uidByte[i] < 0x10) {
          uidString += "0";
        }
        uidString += String(mfrc522.uid.uidByte[i], HEX);
      }
      auto it = uidToName.find(uidString);
      if (it != uidToName.end()) {
        Serial.println("Hello, " + it->second);
      } else {
        Serial.println("Unknown tag!");
      }
      // uidString.toUpperCase();
      Serial.println(uidString);
      Blynk.virtualWrite(V15, uidString);
    }
  } else if (!mfrc522.PICC_IsNewCardPresent()) {
    cardPresent = false; // Reset flag when card is removed
  }
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
  Serial.println("New OTP Generated");
  int value = param.asInt();
  if (value == 1) {
    randomSeed(analogRead(0)); 
    String otp = String(random(10000000, 100000000));
    String formatedOTP = formatDigit(otp);
    Serial2.print("OTP: ");
    Serial2.println(otp);
    Blynk.virtualWrite (V1,formatedOTP);
    sendTelegramMessage(formatedOTP);
  }
}