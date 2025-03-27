const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; // Pins for 10 LEDs
const int potPin = A0; // Potentiometer pin

void setup() {
  for (int i = 0; i < 10; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  int speed = analogRead(potPin) / 4; // Read potentiometer and map to delay value

  for (int i = 0; i < 9; i++) {
    digitalWrite(ledPins[i], HIGH);
    digitalWrite(ledPins[i + 1], HIGH);
    delay(speed);
    digitalWrite(ledPins[i], LOW);
  }
  digitalWrite(ledPins[9], LOW);
}
