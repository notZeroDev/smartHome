const int gasSensorPin = A0;
const int waterSensorPin = A1;

const int led1 = 8;
const int led2 = 9;
const int led3 = 10;

const int gasThreshold = 450;
const int waterThreshold = 250;

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop() {
  int gasValue = analogRead(gasSensorPin);
  int waterValue = analogRead(waterSensorPin);

  Serial.print("Gas: ");
  Serial.print(gasValue);
  Serial.print(" | Water: ");
  Serial.println(waterValue);

  if (gasValue > gasThreshold && waterValue > waterThreshold) {
    handleBothDanger();
  } else if (gasValue > gasThreshold) {
    handleGasDanger();
  } else if (waterValue > waterThreshold) {
    handleWaterDanger();
  } else {
    handleSafeCondition();
  }

  delay(1000);
}

void turnOffLoads() {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
}

void turnOnLoads() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
}

void handleGasDanger() {
  turnOffLoads();
  Serial.println("❌ Dangerous Gas detected! Loads OFF");
}

void handleWaterDanger() {
  turnOffLoads();
  Serial.println("❌ Water Leak detected! Loads OFF");
}

void handleBothDanger() {
  turnOffLoads();
  Serial.println("❌ Both Gas and Water levels are high! Loads OFF");
}

void handleSafeCondition() {
  turnOnLoads();
  Serial.println("✅ Safe conditions. Loads ON");
}
