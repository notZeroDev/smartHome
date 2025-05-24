#include<Arduino.h>
#include"pinout.h"
#include"tones.h"
void success_tone(){
  tone(BuzzerPin, 800);
  delay(150);
  tone(BuzzerPin, 1000);
  delay(150);
  tone(BuzzerPin, 1200);
  delay(200);
  noTone(BuzzerPin);
}
void click_tone(){
  tone(BuzzerPin, 1000);    // 1 kHz tone
  delay(50);                // short beep
  noTone(BuzzerPin);
}
void wrong_tone(){
  for (int i = 0; i < 2; i++) {
    tone(BuzzerPin, 500);   // Lower-pitch error tone
    delay(300);
    noTone(BuzzerPin);
    delay(100);
  }
}
void locked_tone(){
    delay(1000);
    tone(BuzzerPin, 600);
    delay(2000);
    noTone(BuzzerPin); 
}
void otp_tone(){
  tone(BuzzerPin, 1200);
  delay(150);
  tone(BuzzerPin, 1000);
  delay(150);
  tone(BuzzerPin, 800);
  delay(200);
  noTone(BuzzerPin);
}
void reenable_tone() {
  for (int i = 0; i < 2; i++) {
    tone(BuzzerPin, 1500);  // Higher-pitched double beep
    delay(100);
    noTone(BuzzerPin);
    delay(100);
    tone(BuzzerPin, 1500);
    delay(200);
    noTone(BuzzerPin);
    delay(100);
  }
}
