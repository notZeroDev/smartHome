/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp32-mfrc522-rfid-reader-arduino/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.  
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/


#include "../public/env.h"
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
//#include <MFRC522DriverI2C.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h> 
#include <BlynkSimpleEsp32.h>
#include <WiFiClientSecure.h>
bool cardPresent = false; // Tracks if a card has been detected


// Learn more about using SPI/I2C or check the pin assigment for your board: https://github.com/OSSLibraries/Arduino_MFRC522v2#pin-layout
MFRC522DriverPinSimple ss_pin(5);

MFRC522DriverSPI driver{ss_pin}; // Create SPI driver
//MFRC522DriverI2C driver{};     // Create I2C driver
MFRC522 mfrc522{driver};         // Create MFRC522 instance

void setup() {
  Serial.begin(115200);  // Initialize serial communication
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // while (!Serial);       // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4).
  
  mfrc522.PCD_Init();    // Init MFRC522 board.
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);	// Show details of PCD - MFRC522 Card Reader details.
	Serial.println(F("Scan PICC to see UID"));
}

void loop() {
  Blynk.run();
	// If a new card is present and not already marked as present
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    if (!cardPresent) {
      cardPresent = true;  // Mark card as present

      // Serial.print("Card UID: ");
      // MFRC522Debug::PrintUID(Serial, mfrc522.uid);
      // Serial.println();

      // Save UID as string
      String uidString = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (mfrc522.uid.uidByte[i] < 0x10) {
          uidString += "0";
        }
        uidString += String(mfrc522.uid.uidByte[i], HEX);
      }
      Serial.println(uidString);
      Blynk.virtualWrite (V15,uidString);
    }
  } else if (!mfrc522.PICC_IsNewCardPresent()) {
    cardPresent = false; // Reset flag when card is removed
  }
}