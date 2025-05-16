#include "sensorsHandler.h"
#include "pinout.h"




bool led1_state;
bool led2_state;
bool led3_state;


const int gasThreshold = 1800;
const int waterThreshold = 1000;


// last states
bool lg = false;
bool lw = false;

// sensor values 
int gasValue;
int waterValue;
  

void checkWater(){
  waterValue = analogRead(waterSensorPin);
  Serial.print("water sensor: ");
  Serial.println(waterValue);
  Serial.print("!lg: ");
  Serial.println(!lg);
  if (waterValue >= waterThreshold) {
    if(!lw){
      lw = true;
      Serial.println("❌ water Leak detected! Loads OFF"); // replace it with telegram message
    }
  } else{
    if(lw){ 
      Serial.println("water Returned Back to Normal"); // replace it with telegram message
      lw = false;
    }
  }
}
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
void checkSensors(){
  checkWater();
  checkGas();
}
