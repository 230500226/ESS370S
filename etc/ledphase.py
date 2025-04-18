const int led1 = 9; // PWM pin for LED 1
const int led2 = 10; // PWM pin for LED 2
const int led3 = 11; // PWM pin for LED 3

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
int brightness1 = 0; // Brightness of LED 1
int brightness2 = 85; // Brightness of LED 2 (120 degrees out of phase)
int brightness3 = 170; // Brightness of LED 3 (240 degrees out of phase)
int fadeAmount1 = 1; // Amount to change the brightness by for LED 1
int fadeAmount2 = 1; // Amount to change the brightness by for LED 2
int fadeAmount3 = 1; // Amount to change the brightness by for LED 3

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  int interval = 30;

  // Update LED 1
  if (currentMillis - previousMillis1 >= interval) {
    previousMillis1 = currentMillis;
    brightness1 = brightness1 + fadeAmount1;

    if (brightness1 <= 0 || brightness1 >= 255) {
      fadeAmount1 = -fadeAmount1;
    }

    analogWrite(led1, brightness1);
  }

  // Update LED 2
  if (currentMillis - previousMillis2 >= interval) {
    previousMillis2 = currentMillis;
    brightness2 = brightness2 + fadeAmount2;

    if (brightness2 <= 0 || brightness2 >= 255) {
      fadeAmount2 = -fadeAmount2;
    }

    analogWrite(led2, brightness2);
  }

  // Update LED 3
  if (currentMillis - previousMillis3 >= interval) {
    previousMillis3 = currentMillis;
    brightness3 = brightness3 + fadeAmount3;

    if (brightness3 <= 0 || brightness3 >= 255) {
      fadeAmount3 = -fadeAmount3;
    }

    analogWrite(led3, brightness3);
  }
}
