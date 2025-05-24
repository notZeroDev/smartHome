#include <Arduino.h>
#include "demandSide.h"
#include "blynkAPI.h"
#include "telegram.h"
#include "pinout.h"


// Load states
bool acOn = true;
bool heaterOn = true;
bool lightOn = true;
bool messageSent = false;

// Load value
int valAC;
int valHeater;
int valLight;


// Load leds stated
bool pot1 = true,
     pot2 = true,
     pot3 = true;

// Total load threshold
const int totalLoadThreshold = 5000;

;
int getACVal() {
  return analogRead(potAC);
}
int getHeaterVal() {
  return analogRead(potHeater);
}
int getLightVal() {
  return analogRead(potLight);
}

void checkLoad() {
  valAC = getACVal();
  valHeater = getHeaterVal();
  valLight = getLightVal();

  load_sheild();
  updateLoadsStates();
}

void load_sheild() {
  int loadAC = acOn ? valAC : 0;
  int loadHeater = heaterOn ? valHeater : 0;
  int loadLight = lightOn ? valLight : 0;

  int totalLoad = loadAC + loadHeater + loadLight;
  float current = totalLoad * 0.007324219;

  if (current > 25 && pot1 && pot2 && pot3 && enableMessage && !messageSent) {
    sendTelegramMessage("⚠️ System Alert: High Current Load Detected!\n🔌 Total system load has reached 25A.\n⚠️ Warning: If the load continues to increase, the system will automatically shut down the connected loads to prevent damage. Please take immediate action to reduce the load.");
    messageSent = true;  // Prevent further messages
  }

  Serial.print("Total Loads: ");
  Serial.println(totalLoad);
  Serial.print("AC: ");
  Serial.print(valAC);
  Serial.print("   Heater: ");
  Serial.print(valHeater);
  Serial.print("   Light: ");
  Serial.println(valLight);

  bool loadShedThisCycle = cutOffLoad(valAC, valHeater, valLight, totalLoad);

  if (!loadShedThisCycle) {
    restoreLoad(totalLoad, valAC, valHeater, valLight);
  }

  delay(500);
}


// Cascading Cutoff Function: AC → Heater → Light
bool cutOffLoad(int valAC, int valHeater, int valLight, int totalLoad) {
  bool didCut = false;

  // Always cut AC first if it's on and we are over threshold
  if (totalLoad > totalLoadThreshold && acOn) {
    acOn = false;
    //    digitalWrite(ledAC, LOW);
    pot3 = false;
    Serial.println("AC has been cut off.");
    totalLoad -= valAC;
    didCut = true;
  }

  // Then cut Heater if needed
  else if (totalLoad > totalLoadThreshold && heaterOn) {
    heaterOn = false;
    // digitalWrite(ledHeater, LOW);
    pot2 = false;
    Serial.println("Heater has been cut off.");
    totalLoad -= valHeater;
    didCut = true;
  }

  // Finally cut Lights if needed
  else if (totalLoad > totalLoadThreshold && lightOn) {
    lightOn = false;
    pot1 = false;
    Serial.println("Lights have been cut off.");
    didCut = true;
  }

  return didCut;
}

// Restoration Function: Light → Heater → AC
void restoreLoad(int totalLoad, int valAC, int valHeater, int valLight) {

  if (!lightOn) {
    if (totalLoad + valLight <= totalLoadThreshold) {
      lightOn = true;
      pot1 = true;
      Serial.println("Lights restored.");
      totalLoad += valLight;
      return;
    }
  }

  if (lightOn) {
    if (!heaterOn && totalLoad + valHeater <= totalLoadThreshold) {
      heaterOn = true;
      pot2 = true;
      Serial.println("Heater restored.");
      totalLoad += valHeater;
      return;
    }
  }



  if (heaterOn) {
    if (!acOn && totalLoad + valAC <= totalLoadThreshold) {
      acOn = true;
      //    digitalWrite(ledAC, HIGH);
      pot3 = true;
      Serial.println("AC restored.");
      totalLoad += valAC;
      return;
    }
  }
}
