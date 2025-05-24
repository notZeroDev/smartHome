#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>
#include "pinout.h"
#include "tones.h"

const int threshold = 500;  // adjust based on your sensor output (analog value)
bool waterPresent = false;
bool lastWaterState = false;

SoftwareSerial Serial2(SERIAL_RX, SERIAL_TX);  // RX = pin 2, TX = pin 3
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
boolean locked = false;


// functions prototype
void checkSerial();
void checkWater();
void openDoor(String); 
void wrongPass(String = "Wrong Password");
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
  myServo.write(90);
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
  if(locked)return; // do somtheing

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
      delay(500); // small pause before door opens
      print("Correct Password");
      openDoor("Passkey");
      delay(2000);
      pad = "";
      state = true;
    }
    else if (pad == otp){
      delay(500);
      print("OTP Used");
      openDoor("OTP");
      otp = "Z";
      pad = "";
      state = true;
    }
    else {
      wrongPass();
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
  
void openDoor(String method) {
  if(method == String("OTP")) otp_tone();
  else success_tone();
  myServo.write(0);
  Serial2.println(String("7") + method);
  print("Door Opened");
  delay(3000);
  myServo.write(90);
  print("Door Closed");
  delay(1000);
  state = true;
  numTrials = 0;
}
void wrongPass(String msg ="Wrong Password"){
  wrong_tone();
  print(msg);   
  pad = "";
  numTrials++;
  if (numTrials == 3){
    print("LOCKED");
    locked_tone();
    delay(1000);
    state = true;
    Serial2.println("-2");
    locked = true;
  }
}
void checkSerial() {
  if (Serial2.available()) {
    String incoming = Serial2.readStringUntil('\n');

    incoming.trim();  // remove whitespace/newlines
    Serial.print("Receieved FROM esp32: ");
    Serial.println(incoming);
    // Check if message starts with "OTP: "
    if (!locked && incoming.startsWith("0")) { // otp
      // Extract substring a  fter "OTP: " (6 characters)
      otp = incoming.substring(1);
      // Now otp holds the value sent from ESP32
    } else if(!locked && incoming.startsWith("1")){ // nfc
      String person = incoming.substring(1);
      print(String("Welcome ")+ person);
      openDoor("NFC Tag");
    } else if (!locked && incoming.startsWith("-1")){
      wrongPass("unauthorized");
    } else if(!locked && incoming.startsWith("2")){
      print("NFC Enabled");
      delay(2000);
    } else if(!locked && incoming.startsWith("3")){
      print("NFC Disabled");
      delay(2000);
    }else if (incoming.startsWith("9")){
      locked= false;
      numTrials = 0;
      print("Lock Re-Enabled");
      reenable_tone();
      delay(1000);
      state = true;
    }
  }
}

void checkWater(){
  int sensorValue = analogRead(WATER_PIN);

  // Determine if water is detected (above threshold)
  waterPresent = (sensorValue > threshold);

  // Check for rising edge (water detected)
  if (waterPresent && !lastWaterState) {
    Serial2.println("4"); // Water HIGH
    Serial.println("Water Detected");
  }

  // Check for falling edge (water gone)
  if (!waterPresent && lastWaterState) {
    Serial2.println("5"); // Water LOW
  }

  lastWaterState = waterPresent;
}
