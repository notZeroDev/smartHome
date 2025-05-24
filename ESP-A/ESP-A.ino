#include "demandSide.h"
#include "sensorsHandler.h"
#include "pinout.h"
#include "env.h"
// #include <BlynkSimpleEsp32.h>
#include "blynkAPI.h"
#include <WiFiClientSecure.h>
#include "https.h"
#include "telegram.h"
#include "RFID.h"
#include <string>



// Global Variables
bool enableLogs = true;
void readRFID();
void checkSerial();
bool nfcEnabled = false;


void setup() {

  // Serial motnitor setup
  Serial.begin(115200);
  Serial.println("HELLO FROM SERIAL");

  // RFID setup
  RFIDSetup();

  // blynk
  blynkSetup();


  // Serial communcation with Arduino
  //! update it to pinout.h
  Serial2.begin(115200, SERIAL_8N1, SERIAL_RX, SERIAL_TX);  // RX=GPIO16, TX=GPIO17

  // telegram setup
  setupTelegramBot();
  sendTelegramMessage("Smart Home Started From ESP32");

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
}

void loop() {

  checkSerial();


  // blynk
  blynkRun();

  // demand side managment
  checkLoad();
  updateLoadsStates();
  digitalWrite(ledAC, !pot3);
  digitalWrite(ledHeater, !pot2);
  digitalWrite(ledLight, !pot1);

  // sensor handlres
  checkGas();
  readRFID();
}

void checkSerial() {
  if (Serial2.available()) {
    String incoming = Serial2.readStringUntil('\n');
    incoming.trim();
    if (incoming.startsWith("4")) {  // water high
      Serial.println("Water Leackage Is Detected");
      sendTelegramMessage("⚠️ Water Has been detected!");
      pot1 = false;
      updateLightLoad(false);              // cut off the light load
    } else if (incoming.startsWith("-2"))  // intruder
      sendTelegramMessage("ALERT: Door locked after 3 failed tries.\nOpen Blynk app or Watch Live feed: http://192.168.249.94");
    else if (incoming.startsWith("7")) {  // unlocked: method
      String method = incoming.substring(1);
      Serial.println(String("Door Oopend using: ") + method);
      if (enableLogs)
        makeHTTPRequest("https://maker.ifttt.com/trigger/Smart_Home_Logs/with/key/" + String(IFTTT_AUTH) + "?value1=" + method);
    }
  }
}