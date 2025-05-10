#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>
#define BLYNK_TEMPLATE_NAME "smartlock"
#define BLYNK_TEMPLATE_ID "TMPL2wVMUtUFu"
#define BLYNK_AUTH_TOKEN "5qufVBDcSKXd2EKvK2DM9Gn_ZnJTQtgq"
#define BOT_TOKEN "7844471260076333:AAEJJnANcxreJhfJfBQ4fPvfIjJybNUuN5I""
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#define CHAT_ID "Telegram-User-ID"
#define BuzzerPin 19 
char ssid[] = "wifi-ssid";
char pass[] = "wifi-password";
const char allowed[] = "ABCD0123456789";

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

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  myServo.attach(18);
  myServo.write(0);
  pinMode(BuzzerPin,OUTPUT);
}

void loop() {
	Blynk.run();
	smartLock();
}
  


void smartLock() {
	if (numTrials == 3){
  		lcd.setCursor(0,0);
    	lcd.print("LOCKED");
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
    	lcd.clear();
    	lcd.setCursor(0,0);
    	lcd.print("Password:");
    	lcd.setCursor(0, 1);
    	lcd.print(pad);
      	delay(100);
    }

	if (pad == HomePassword){
    	lcd.clear();
    	lcd.setCursor(0,0);
    	lcd.print("Correct Password");
    	delay(1000);
  		openDoor();
    	pad = "";
  	}

    else if (pad == otp){
  		lcd.clear();
    	lcd.setCursor(0,0);
    	lcd.print("OTP Used");
		openDoor();
		otp = "";
	}

  	else if (pad.length() > 6) {
  		lcd.clear();
    	lcd.setCursor(0,0);
    	lcd.print("Wrong password");
    	delay(1000);
   		tone(BuzzerPin, 600);
    	delay(700);
    	noTone(BuzzerPin);     
   		pad = "";
    	lcd.clear();
    	numTrials++;
		Blynk.virtualWrite (V1, "The password has been entered unsuccessfully");
  	}

	else if (pad == "") {
		lcd.setCursor(0,0);
    	lcd.print("Enter Passowrd: ");
	}
}

void openDoor() {
	lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Door Is Opening");
  	for (int pos = 0; pos <= 90; pos++) {
    	myServo.write(pos);
    	delay(10);
  	}
	lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Door Opened");
	delay(30000);
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
  	otp="";
  	for (int i = 0; i < 6; i++) {
  		int r = random(0, 14);
  		otp += allowed[r];
  	}
  Blynk.virtualWrite (V1, otp);
  bot.sendMessage(CHAT_ID, otp, "");
  delay(500);
  }
}