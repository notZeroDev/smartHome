#include <Arduino.h>
//#include "../public/env.h"

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
// #include "../public/helper.h"



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
void openDoor(); 
void smartLock();
void print(String, int=0);
String formatDigit(String);

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Serial.begin(115200);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  myServo.attach(SERVO_PIN);
  myServo.write(0);
  pinMode(BuzzerPin,OUTPUT);

  
}

void loop() {
  // Blynk.run();
  smartLock();
//  tone(BuzzerPin, 1000);  // Play 1 kHz tone
//  delay(500);              // Wait half a second
//  noTone(BuzzerPin);      // Stop sound
//  delay(500);              // Wait again
}

void print(String str1, int len) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(str1);
//  Serial.println(str1);
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
    // Blynk.virtualWrite (V1, "Intruder Alert");
    delay(1000);
    tone(BuzzerPin, 600);
    delay(2000);
    noTone(BuzzerPin); 
    return;
  }

  char customKey = customKeypad.getKey();
if (customKey){
  tone(BuzzerPin, 1000);    // 1 kHz tone
  delay(50);                // short beep
  noTone(BuzzerPin);

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
      tone(BuzzerPin, 800);
delay(150);
tone(BuzzerPin, 1000);
delay(150);
tone(BuzzerPin, 1200);
delay(200);
noTone(BuzzerPin);

delay(500); // small pause before door opens
      print("Correct Password");
      delay(2000);
      openDoor();
      pad = "";
      state = true;
    }
    else if (pad == otp){
      tone(BuzzerPin, 1200);
      delay(150);
      tone(BuzzerPin, 1000);
      delay(150);
      tone(BuzzerPin, 800);
      delay(200);
      noTone(BuzzerPin);
    
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
      tone(BuzzerPin, 500);   // Lower-pitch error tone
      delay(300);
      noTone(BuzzerPin);
      delay(100);
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
    if (pad.length() < 6) {
      pad += customKey;
    }
    else {
      print("Max 6 digits");
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

// BLYNK_WRITE(V0) {
//   int value = param.asInt();
//   if (value == 1) {
//   randomSeed(analogRead(0)); 
//   otp = String(random(10000000, 100000000));
//   String formatedOTP = formatDigit(otp);
//   Blynk.virtualWrite (V1,formatedOTP);
//   bot.sendMessage(CHAT_ID,formatedOTP, "");
//   delay(500);
//   }
// }
