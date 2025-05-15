
// Potentiometer pins
const int potAC = A0;
const int potHeater = A1;
const int potLight = A2;

// LED pins
const int ledAC = 4;
const int ledHeater = 5;
const int ledLight = 6;

// Load states
bool acOn = true;
bool heaterOn = true;
bool lightOn = true;

// Load value
int valAC;
int valHeater;
int valLight;


// Load leds stated
bool pot1, pot2, pot3;

// Total load threshold
const int totalLoadThreshold = 500;

void setup() {
  Serial.begin(9600);
  

  pinMode(ledAC, OUTPUT);
  pinMode(ledHeater, OUTPUT);
  pinMode(ledLight, OUTPUT);

//  digitalWrite(ledAC, HIGH);
//  digitalWrite(ledHeater, HIGH);
//  digitalWrite(ledLight, HIGH);

 Serial.println("System started. All devices ON.");

}

void loop() {
  valAC = analogRead(potAC);        // Now updates the global variable
  valHeater = analogRead(potHeater);
  valLight = analogRead(potLight);
  
  load_shed();
  digitalWrite(ledAC, pot3);
  digitalWrite(ledHeater, pot2);
  digitalWrite(ledLight, pot1);
}

void load_shed(){
  
  int loadAC = acOn ? valAC : 0;
  int loadHeater = heaterOn ? valHeater : 0;
  int loadLight = lightOn ? valLight : 0;

  int totalLoad = loadAC + loadHeater + loadLight;
  Serial.print("Total Loads: ");
  Serial.println(totalLoad);
  Serial.print("AC: ");
  Serial.print(valAC);
  Serial.print("   Heater: ");
  Serial.print(valHeater);
  Serial.print("   Light: ");
  Serial.println(valLight);
  // Serial.print("Total Load: ");
  // Serial.println(totalLoad);

  bool loadShedThisCycle = cutOffLoad(valAC, valHeater, valLight, totalLoad);

  if (!loadShedThisCycle) {
    restoreLoad(totalLoad,valAC, valHeater, valLight);
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
//    digitalWrite(ledHeater, LOW);
    pot2 = false;
    Serial.println("Heater has been cut off.");
    totalLoad -= valHeater;
    didCut = true;
  }

  // Finally cut Lights if needed
  else if (totalLoad > totalLoadThreshold && lightOn) {
    lightOn = false;
//    digitalWrite(ledLight, LOW);
    pot1 = false;
    Serial.println("Lights have been cut off.");
    didCut = true;
  }

  return didCut;
}

// Restoration Function: Light → Heater → AC
void restoreLoad(int totalLoad, int valAC, int valHeater, int valLight) {
  
  if (!lightOn ){
    if (totalLoad + valLight <= totalLoadThreshold ) {
    lightOn = true;
//    digitalWrite(ledLight, HIGH);
    pot1 = true;
    Serial.println("Lights restored.");
    totalLoad+=valLight;
    return;
  }
  }
 
  if(lightOn)
  {
  if (!heaterOn && totalLoad + valHeater <= totalLoadThreshold) {
    heaterOn = true;
//    digitalWrite(ledHeater, HIGH);
    pot2 = true;
    Serial.println("Heater restored.");
    totalLoad+=valHeater;
    return;
  }
  }
  

 
  if(heaterOn) {
    if (!acOn && totalLoad + valAC <= totalLoadThreshold) {
    acOn = true;
//    digitalWrite(ledAC, HIGH);
    pot3 = true;
    Serial.println("AC restored.");
      totalLoad+=valAC;
    return;
    }
  
  }
}
