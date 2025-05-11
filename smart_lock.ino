#include <Arduino.h>
#include "env.h"
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
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
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;
byte numTrials = 0;
const byte Rows = 4;
const byte Cols = 4;
char hexakeys[Rows][Cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},  
};
byte rowPins[Rows] = {13, 12, 14, 27};
byte colPins[Cols] = {26, 25, 33, 32};
Keypad customKeypad = Keypad(makeKeymap(hexakeys), rowPins, colPins, Rows, Cols);
String pad = "";
String HomePassword = "123";
String otp = "ZZZ";

boolean state =true;
// functions prototype
void openDoor(); 
void smartLock();
void print(String, String);
void makeTelegramCall(String);
String formatDigit(long);

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  myServo.attach(18);
  myServo.write(0);
  pinMode(BuzzerPin,OUTPUT);
  makeTelegramCall("Testing calls from ESP 32");
}

void loop() {
	Blynk.run();
	smartLock();
}
void print(String str1, String str2 = ""){
  // lcd.clear();
	// lcd.setCursor(0,0);
  // lcd.print(str1);
  // if(str2!=""){
	// lcd.setCursor(0,1);
  // lcd.print(str2);
  // }
  Serial.println(str1);
  if (str2!="") Serial.println(str2);
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
String formatDigit(long input){
  String str = String(input);
  String output = "";
  int len = str.length();

  for (int i = 0; i < len; i++) {
    output += str[i];
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
		pad += customKey; 
    	//Serial.println(customKey);
      print("Password:", pad);
      delay(100);
    }

	if (pad == HomePassword){
    print("Correct Password");
    delay(1000);
    openDoor();
    pad = "";
    state = true;
  }

    else if (pad == otp){
      print("OTP Used");
		openDoor();
		otp = "";
    state = true;
	}

	else if (pad.length() > 6) {
      print("Wrong password");
      delay(1000);
      tone(BuzzerPin, 600);
      delay(700);
      noTone(BuzzerPin);     
      pad = "";
      lcd.clear();
      numTrials++;
      Blynk.virtualWrite (V1, "The password has been entered unsuccessfully");
      state = true;
    }
	else if (state){
    print("Enter Passowrd: ");
    state = false;
	}
}

void openDoor() {
  print("Door Is Opening");
  myServo.write(90);
	print("Door Opened");
	delay(30000);
	print("Door Is Closing");
  myServo.write(0);
	print("Door Closed");
	Blynk.virtualWrite (V1, "Someone Opened the Door");
}

/*void closeDoor() {
	lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Door Is Closing");
  	for (int pos = 90; pos >= 0; pos--) {
    	myServo.write(pos);
    	delay(10);
 	}
	lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Door Closed");
	delay(500);
}*/

BLYNK_WRITE(V0) {
  int value = param.asInt();
  if (value == 1) {
  randomSeed(analogRead(0)); 
  otp = formatDigit(random(10000000, 100000000));
  Blynk.virtualWrite (V1, otp);
  bot.sendMessage(CHAT_ID, otp, "");
  delay(500);
  }
}