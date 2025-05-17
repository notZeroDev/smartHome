void setup() {
  Serial.begin(9600);  // default on Serial0 (USB)
  Serial1.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17 (change pins if needed)
  // RX pin (16) gets data from Arduino TX (through divider)
  // TX pin (17) goes to Arduino RX directly
}

void loop() {
  if (Serial1.available()) {
    char c = Serial1.read();
    Serial.print("ESP32 received: ");
    Serial.println(c);
  }

  // Send message to Arduino every 2 sec
  Serial1.println("Hello from ESP32");
  delay(2000);
}
