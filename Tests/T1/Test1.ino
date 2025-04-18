// Define constants for PWM and static pins
const int pwmPins[] = {3, 5, 6, 9, 10, 11}; // PWM pins
const int staticPins[] = {2, 4, 7, 8, 12, 13}; // Static pins

// Brightness levels for PWM LEDs
int brightness[] = {0, 42, 84, 126, 168, 255}; // Initial brightness
const int increment = 1; // Increment for brightness change

// Timing for static LEDs
unsigned long staticLedStartTimes[6] = {0, 0, 0, 0, 0, 0};
const unsigned long staticLedDuration = 1000; // 1 second duration

void setup() {
  // Set up PWM pins as output
  for (int i = 0; i < 6; i++) {
    pinMode(pwmPins[i], OUTPUT);
    analogWrite(pwmPins[i], brightness[i]); // Set initial brightness
  }

  // Set up static pins as output
  for (int i = 0; i < 6; i++) {
    pinMode(staticPins[i], OUTPUT);
    digitalWrite(staticPins[i], LOW); // Turn off static LEDs initially
  }
}

void loop() {
  unsigned long currentTime = millis();

  // Update brightness of each PWM LED
  for (int i = 0; i < 6; i++) {
    brightness[i] += increment;

    // Check if PWM LED reaches full brightness
    if (brightness[i] > 255) {
      brightness[i] = 0; // Reset brightness

      // Turn on corresponding static LED for 1 second
      digitalWrite(staticPins[i], HIGH);
      staticLedStartTimes[i] = currentTime;
    }

    // Update PWM LED brightness
    analogWrite(pwmPins[i], brightness[i]);
  }

  // Turn off static LEDs after 1 second
  for (int i = 0; i < 6; i++) {
    if (currentTime - staticLedStartTimes[i] >= staticLedDuration) {
      digitalWrite(staticPins[i], LOW);
    }
  }

  delay(10); // Small delay for smooth brightness transition
}
