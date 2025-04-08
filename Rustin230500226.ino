const int staticNumLeds = 6;
const int dynNumLeds = 6;
int staticLedPins[staticNumLeds] = {2, 4, 7, 8, 12, 13};
int dynLedPins[dynNumLeds] = {3, 5, 6, 9, 10, 11};

unsigned long previousMillis1 = 0;
unsigned long previousMillis2[dynNumLeds] = {0};
int brightness[dynNumLeds] = {0, 43, 85, 128, 170, 213}; // Adjusted values to be integers
int fadeAmount = 5; // Amount to change the brightness, adjusted for smoother fading

void setup() {
  for (int i = 0; i < staticNumLeds; i++) {
    pinMode(staticLedPins[i], OUTPUT);
  }
  for (int i = 0; i < dynNumLeds; i++) {
    pinMode(dynLedPins[i], OUTPUT);
  }
}

void loop() {
  unsigned long currentMillis = millis();
  int interval = 30;

  if (currentMillis - previousMillis1 >= interval) {
    previousMillis1 = currentMillis;

    for (int i = 0; i < dynNumLeds; i++) {
      brightness[i] = brightness[i] + fadeAmount;

      if (brightness[i] >= 255) {
        brightness[i] = 0;
        digitalWrite(staticLedPins[i], HIGH);
        previousMillis2[i] = currentMillis;
      }
      analogWrite(dynLedPins[i], brightness[i]);
    }
  }

  for (int i = 0; i < staticNumLeds; i++) {
    if (digitalRead(staticLedPins[i]) == HIGH && (currentMillis - previousMillis2[i] >= 400)) {
      digitalWrite(staticLedPins[i], LOW);
    }
  }
}
