#ifndef TELEGRAM_H
#define TELEGRAM_H
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

extern WiFiClientSecure secured_client;
extern UniversalTelegramBot bot;
extern bool enableMessage;
extern bool enableCalls;
void setupTelegramBot();
void makeTelegramCall(String msg);
void sendTelegramMessage(String msg);

#endif