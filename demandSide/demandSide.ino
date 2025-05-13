  // Potentiometer pins
const int potAC = 32;
const int potHeater = 33;
const int potLight = 34;

// LED pins
const int ledAC = 25;
const int ledHeater = 26;
const int ledLight = 27;

// Load states
bool acOn = true;
bool heaterOn = true;
bool lightOn = true;

// Total load threshold
const int totalLoadThreshold = 6000;

void setup() {
  Serial.begin(9600);

  pinMode(ledAC, OUTPUT);
  pinMode(ledHeater, OUTPUT);
  pinMode(ledLight, OUTPUT);

  digitalWrite(ledAC, LOW);
  digitalWrite(ledHeater, LOW);
  digitalWrite(ledLight, LOW);

  Serial.println("System started. All devices ON.");
}

void loop() {
  int valAC = analogRead(potAC);
  int valHeater = analogRead(potHeater);
  int valLight = analogRead(potLight);
  Serial.println("Total Loads");
  // Serial.print("AC: ");
  // Serial.print(valAC);
  // Serial.print("   Heater: ");
  // Serial.print(valHeater);
  // Serial.print("   Light: ");
  // Serial.println(valLight);
  int loadAC = acOn ? valAC : 0;
  int loadHeater = heaterOn ? valHeater : 0;
  int loadLight = lightOn ? valLight : 0;

  int totalLoad = loadAC + loadHeater + loadLight;

  Serial.print("Total Load: ");
  Serial.println(totalLoad);

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
    digitalWrite(ledAC, HIGH);
    Serial.println("AC has been cut off.");
    totalLoad -= valAC;
    didCut = true;
  }

  // Then cut Heater if needed
  else if (totalLoad > totalLoadThreshold && heaterOn) {
    heaterOn = false;
    digitalWrite(ledHeater, HIGH);
    Serial.println("Heater has been cut off.");
    totalLoad -= valHeater;
    didCut = true;
  }

  // Finally cut Lights if needed
  else if (totalLoad > totalLoadThreshold && lightOn) {
    lightOn = false;
    digitalWrite(ledLight, HIGH);
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
    digitalWrite(ledLight, LOW);
    Serial.println("Lights restored.");
    totalLoad+=valLight;
    return;
  }
  }
 
  if(lightOn)
  {
  if (!heaterOn && totalLoad + valHeater <= totalLoadThreshold) {
    heaterOn = true;
    digitalWrite(ledHeater, LOW);
    Serial.println("Heater restored.");
    totalLoad+=valHeater;
    return;
  }
  }
  

 
  if(heaterOn) {
    if (!acOn && totalLoad + valAC <= totalLoadThreshold) {
    acOn = true;
    digitalWrite(ledAC, LOW);
    Serial.println("AC restored.");
      totalLoad+=valAC;
    return;
    }
  
  }
}
