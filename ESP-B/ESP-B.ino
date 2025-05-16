#include <Arduino.h>
#include "../env.h"
#include <Keypad.h>
// #include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <HTTPClient.h>
#define BuzzerPin 19 

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
// LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;
byte numTrials = 0;
const byte Rows = 4;
const byte Cols = 4;
char hexakeys[Rows][Cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'},  
};
byte rowPins[Rows] = {13, 12, 14, 27};
byte colPins[Cols] = {26, 25, 33, 32};
Keypad customKeypad = Keypad(makeKeymap(hexakeys), rowPins, colPins, Rows, Cols);
String pad = "";
String HomePassword = "123";
String otp = "Z";
String hiddenPad = "";
boolean state =true;
// functions prototype
void openDoor(); 
void smartLock();
void print(String, int=0);
void makeTelegramCall(String);
String formatDigit(String);

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  Wire.begin(21, 22);
  // lcd.init();
  // lcd.backlight();
  myServo.attach(18);
  myServo.write(0);
  pinMode(BuzzerPin,OUTPUT);
  makeTelegramCall("Testing calls from ESP 32");
  bot.sendMessage(CHAT_ID,"Hello from ESP32", "");
}

void loop() {
	Blynk.run();
	smartLock();
}

void print(String str1, int len) {
  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print(str1);
  Serial.println(str1);
  hiddenPad = "";
  if (len > 0 && len <= 6) {
	for (int i = 0; i < len; i++) {
  	   hiddenPad += '*';
    }
  // lcd.setCursor(0, 1);
  // lcd.print(hiddenPad);
  }
}

void makeTelegramCall(String msg){
  String encodedMsg = "";
  for (int i = 0; i < msg.length(); i++) {
    char c = msg.charAt(i);
    if (c == ' ') {
      encodedMsg += "%20";
    } else {
      encodedMsg += c;
    }
  }

  String url = "http://api.callmebot.com/start.php?source=web&user=" + String(TELEGRAM_USER) + "&text=" + encodedMsg + "&lang=en-GB-Standard-B";
  print(url);
  // Send HTTP GET request
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);  // Prepare URL
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String response = http.getString();
      Serial.println("Response:");
      Serial.println(response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
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
void smartLock() {
	if (numTrials == 3){
    print("LOCKED");
		Blynk.virtualWrite (V1, "Intruder Alert");
		delay(1000);
    tone(BuzzerPin, 600);
    delay(2000);
    noTone(BuzzerPin); 
		return;
  }

	char customKey = customKeypad.getKey();

if (customKey){

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
      print("Correct Password");
      delay(2000);
      openDoor();
      pad = "";
      state = true;
    }
    else if (pad == otp){
      print("OTP Used");
      openDoor();
      otp = "Z";
      pad = "";
      state = true;
      bot.sendMessage(CHAT_ID,"OTP has been used", "");
    }
    else {
      print("Wrong password");
      delay(1000);
      tone(BuzzerPin, 600);
      delay(700);
      noTone(BuzzerPin);     
      pad = "";
      // lcd.clear();
      numTrials++;
      Blynk.virtualWrite (V1, "The password has been entered unsuccessfully");
      state = true;
    }
  }

  else {  // the key entered is neither * nor # 
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
	delay(30000);
  myServo.write(0);
	print("Door Closed");
	Blynk.virtualWrite (V1, "Someone Opened the Door");
  numTrials = 0;
}

BLYNK_WRITE(V0) {
  int value = param.asInt();
  if (value == 1) {
  randomSeed(analogRead(0)); 
  otp = String(random(10000000, 100000000));
  String formatedOTP = formatDigit(otp);
  Blynk.virtualWrite (V1,formatedOTP);
  bot.sendMessage(CHAT_ID,formatedOTP, "");
  delay(500);
  }
}