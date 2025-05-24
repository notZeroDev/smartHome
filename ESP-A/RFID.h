#ifndef RFID_H
#define RFID_H

extern bool cardPresnet;
extern bool enableLogs;
extern bool nfcEnabled;

void RFIDSetup();
void readRFID();

#endif