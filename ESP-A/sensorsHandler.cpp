#include "sensorsHandler.h"
#include "pinout.h"
#include "telegram.h"
#include "blynkAPI.h"



bool led1_state;
bool led2_state;
bool led3_state;


const int gasThreshold = 1800;


// last states
bool lg = false;
bool lw = false;

// sensor values
int gasValue;

void checkGas() {
  gasValue = analogRead(gasSensorPin);
  if (gasValue >= gasThreshold) {
    if (!lg) {
      // Gas Detected
      lg = true;
      pot1 = false;
      pot2 = false;
      pot3 = false;
      // cut off all loads
      updateLoadsVirtualPins(false, false, false);
      if (enableCalls)
        makeTelegramCall("Gas Leakage in your house");
      if (enableMessage)
        sendTelegramMessage("Gas Leakage in your house");
    }
  } else {
    if (lg) {
      lg = false;
    }
  }
}