void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    Serial.print("Arduino received: ");
    Serial.println(c);
  }

  // Send something to ESP32 every 2 sec
  Serial.println("Hello from Arduino");
  delay(2000);
}
