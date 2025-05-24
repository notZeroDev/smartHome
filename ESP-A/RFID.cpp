#include <Arduino.h>
#include "RFID.h"
#include "https.h"
#include "env.h"
#include "pinout.h"
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>
#include <map>

bool cardPresent = false;
std::map<String, String> uidToName = {
  { "40625b1b", "Mohamed" },
  { "819b4b27", "Marwan" },
};

// Create the ss_pin object before driver
MFRC522DriverPinSimple ss_pin(rfidSS);
MFRC522DriverSPI driver(ss_pin, SPI);
MFRC522 mfrc522(driver);
void RFIDSetup() {
  SPI.begin(spiSCK, spiMISO, spiMOSI);
  mfrc522.PCD_Init();  // Init MFRC522 board.
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);
  Serial.println(F("Scan PICC to see UID"));
}
void readRFID() {
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
      if (nfcEnabled && it != uidToName.end()) {
        Serial2.println("1" + it->second);  // nfc
        if (enableLogs)
          makeHTTPRequest("https://maker.ifttt.com/trigger/Smart_Home_Logs/with/key/" + String(IFTTT_AUTH) + "?value1=NFC");
      } else {
        Serial2.println("-1");  // unauthorized
      }
      Serial.println(uidString);
    }
  } else if (!mfrc522.PICC_IsNewCardPresent()) {
    cardPresent = false;  // Reset flag when card is removed
  }
}