#ifndef BLYNK_API_H
#define BLYNK_API_H
// extern BlynkWifi Blynk;
extern bool enableCalls;
extern bool enableLogs;
extern bool enableMessage;
extern bool nfcEnabled;

void blynkSetup();
void blynkRun();
void updateACLoad(bool);
void updateHeaterLoad(bool);
void updateLightLoad(bool);
void updateLoadsVirtualPins(bool ACLoad, bool heaterLoad, bool lightLoad);
void updateLoadsStates();
void updateVirtualPin(int pin, auto data);
#endif