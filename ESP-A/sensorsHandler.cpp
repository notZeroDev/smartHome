#include "sensorsHandler.h"
#include "pinout.h"




bool led1_state;
bool led2_state;
bool led3_state;


const int gasThreshold = 1800;


// last states
bool lg = false;
bool lw = false;

// sensor values 
int gasValue;
  


void checkGas(){
  gasValue = analogRead(gasSensorPin);
//  Serial.print("Gas sensor: ");
//  Serial.println(gasValue);
//  Serial.print("!lg: ");
//  Serial.println(!lg);
  if (gasValue >= gasThreshold) {
    if(!lg){
      lg = true;
      Serial.println("❌ GAS Leak detected! Loads OFF"); // replace it with telegram message
    }
  } else{
    if(lg){ 
      Serial.println("Gas Returned Back to Normal"); // replace it with telegram message
      lg = false;
    }
  }
}
