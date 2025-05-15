const int gasSensorPin = A0;
const int waterSensorPin = A1;

const int led1 = 8;
const int led2 = 9;
const int led3 = 10;

bool led1_state;
bool led2_state;
bool led3_state;


const int gasThreshold = 450;
const int waterThreshold = 250;


// last states
bool lg = false;
bool lw = false;

// sensor values 
int gasValue;
int waterValue;
  
void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop() {
  checkSensors();
  delay(1000);
}
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
  Serial.print("Gas sensor: ");
  Serial.println(gasValue);
  Serial.print("!lg: ");
  Serial.println(!lg);
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
  cuttLoads();
}
void cuttLoads(){
  digitalWrite(led1, (!lg && !lw)); // reverse the logic for relay(Active LOW)
  digitalWrite(led2, (!lg));
  digitalWrite(led3, (!lg));
}
