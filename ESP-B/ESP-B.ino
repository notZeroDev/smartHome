#include <Arduino.h>
//#include "../public/env.h"
#include <SoftwareSerial.h>

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>
// #include <WiFiClientSecure.h>
// #include <UniversalTelegramBot.h>
// #include <WiFi.h>
// #include <BlynkSimpleEsp32.h>
// #include <HTTPClient.h>
// #include "../public/telegram.ino"
#include "pinout.h"
#include "tones.h"
// #include "../public/helper.h"

const int threshold = 500;  // adjust based on your sensor output (analog value)
bool waterPresent = false;
bool lastWaterState = false;

SoftwareSerial Serial2(2, 3);  // RX = pin 2, TX = pin 3
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;
byte numTrials = 0;
char hexakeys[Rows][Cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'},  
};

Keypad customKeypad = Keypad(makeKeymap(hexakeys), rowPins, colPins, Rows, Cols);
String pad = "";
String HomePassword = "147";
String otp = "111";
String hiddenPad = "";
boolean state =true;


// functions prototype
void checkSerial();
void checkWater();
void openDoor(); 
void smartLock();
void print(String, int=0);
String formatDigit(String);

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  myServo.attach(SERVO_PIN);
  myServo.write(0);
  pinMode(BuzzerPin,OUTPUT);
  pinMode(WATER_PIN, INPUT);
  
}

void loop() {
  checkSerial();
  checkWater();
  smartLock();
}

void print(String str1, int len) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(str1);
  hiddenPad = "";
  if (len > 0 && len <= 6) {
  for (int i = 0; i < len; i++) {
       hiddenPad += '*';
    }
  lcd.setCursor(0, 1);
  lcd.print(hiddenPad);
  }
}



void smartLock() {
  if (numTrials == 3){
    print("LOCKED");
    locked_tone();
    return;
  }

  char customKey = customKeypad.getKey();
  if (customKey){
  click_tone();

  if(customKey=='*'){  //if the key pressed is * we will delete one character from pad
    if(pad.length() <= 1){  //if pad already has one or zero chars
      state = true;  //the screen will say enter password
      pad="";
      }
    else{
      pad.remove(pad.length() - 1); // else we have more than 1 char then we will delete the last char entered
      print("Password:", pad.length());
      delay(100);
      }
    }

  else if (customKey=='#'){ // else if # is entered we go check if password is correct
    if (pad == HomePassword){
      success_tone();
      delay(500); // small pause before door opens
      print("Correct Password");
      delay(2000);
      openDoor();
      pad = "";
      state = true;
    }
    else if (pad == otp){
      otp_tone();
      delay(500);
      print("OTP Used");
      openDoor();
      otp = "Z";
      pad = "";
      state = true;
      // sendTelegramMessage("OTP has been used");
    }
    else {
      
      for (int i = 0; i < 2; i++) {
      wrong_tone();
      }
      print("Wrong password");   
      pad = "";
      // lcd.clear();
      numTrials++;
      // Blynk.virtualWrite (V1, "The password has been entered unsuccessfully");
      state = true;
    }
  }

  else {  // the key entered is neither * nor # 
    Serial.println(customKey);
    if (pad.length() < 8) {
      pad += customKey;
    }
    else {
      print("Max 8 digits");
      delay(2000);
    }
    print("Password:", pad.length());
    state = false;
  }
}
    
if (state){
        print("Enter Password: ");
        state = false;
  }
}
  
void openDoor() {
  myServo.write(90);
  print("Door Opened");
  delay(3000);
  myServo.write(0);
  print("Door Closed");
  // Blynk.virtualWrite (V1, "Someone Opened the Door");
  numTrials = 0;
}
void checkSerial() {
  if (Serial2.available()) {
    String incoming = Serial2.readStringUntil('\n');

    incoming.trim();  // remove whitespace/newlines
    Serial.print("Receieved FROM esp32: ");
    Serial.println(incoming);
    // Check if message starts with "OTP: "
    if (incoming.startsWith("OTP: ")) {
      // Extract substring after "OTP: " (6 characters)
      otp = incoming.substring(5);
      // Now otp holds the value sent from ESP32
    }
  }
}
void checkWater(){
  int sensorValue = analogRead(WATER_PIN);

  // Determine if water is detected (above threshold)
  waterPresent = (sensorValue > threshold);

  // Check for rising edge (water detected)
  if (waterPresent && !lastWaterState) {
    Serial2.println("WATER HIGH");
  }

  // Check for falling edge (water gone)
  if (!waterPresent && lastWaterState) {
    Serial2.println("WATER LOW");
  }

  lastWaterState = waterPresent;
}
