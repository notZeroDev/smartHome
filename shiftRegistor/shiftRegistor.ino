// Pin Definitions
const int dataPin = 6;   // DS
const int clockPin = 4;  // SHCP
const int latchPin = 5;  // STCP

// Output state for all 16 pins (2 x 8-bit shift registers)
bool outputStates[16] = {0};  // Initialized to LOW

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  // Optional: set all outputs LOW at startup
  sendShiftRegisterOutput();
}

// Main loop (example demo)
void loop() {
  /*
  // Example: light up each pin one-by-one
  for (int i = 0; i < 16; i++) {
    setOutputPin(i, HIGH);
    delay(100);
  }

  // Then turn them off
  for (int i = 0; i < 16; i++) {
    setOutputPin(i, LOW);
    delay(100);
  }
  */
  setOutputPin(0, 1);
  setOutputPin(12, 1);
  setOutputPin(13, 1);
  delay(500);
  setOutputPin(0, 0);
  setOutputPin(12, 0);
  setOutputPin(13, 0);
  delay(500);


}

// Function to set a single pin and keep others unchanged
void setOutputPin(int pinIndex, bool state) {
  if (pinIndex < 0 || pinIndex >= 16) return;  // Out-of-range safety
  outputStates[15 - pinIndex] = state;
  sendShiftRegisterOutput();
}

// Function to shift out all 16 bits (MSB first)
void sendShiftRegisterOutput() {
  digitalWrite(latchPin, LOW);  // Begin shifting

  // Shift out from LSB to MSB (Q0 → Q15)
  for (int i = 0; i < 16; i++) {
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, outputStates[i] ? HIGH : LOW);
    digitalWrite(clockPin, HIGH);
  }

  digitalWrite(latchPin, HIGH); // Latch data to outputs
}

