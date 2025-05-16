#include <HTTPClient.h>
#include <UniversalTelegramBot.h>
#include "env.h"
#include <Arduino.h>

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

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
void sendTelegramMessage(String msg){// wifi telegram

  bot.sendMessage(CHAT_ID, msg, "");
}