#include "telegram.h"
#include <HTTPClient.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include "env.h"
#include "https.h"
#include <Arduino.h>

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
bool enableMessage = true;
bool enableCalls = true;
void setupTelegramBot() {
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
}
void makeTelegramCall(String msg) {
  if (!enableCalls) return;
  String encodedMsg = "";
  for (int i = 0; i < msg.length(); i++) {
    char c = msg.charAt(i);
    if (c == ' ') {
      encodedMsg += "%20";
    } else {
      encodedMsg += c;
    }
  }
  makeHTTPRequest("http://api.callmebot.com/start.php?source=web&user=" + String(TELEGRAM_USER) + "&text=" + encodedMsg + "&lang=en-GB-Standard-B");
}
void sendTelegramMessage(String msg) {
  if (!enableMessage) return;
  bot.sendMessage(CHAT_ID, msg, "");
}