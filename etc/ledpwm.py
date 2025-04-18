const int led1 = 9; // PWM pin for LED 1
const int led2 = 10; // PWM pin for LED 2
const int buttonPin = 2; // Pin for the button

unsigned long previousMillis = 0;
const long interval = 1000; // interval at which to change LED brightness (milliseconds)
int brightness = 0; // initial brightness level
int fadeAmount = 5; // amount to change the brightness

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Set the button pin as input with internal pull-up resistor
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if the button is pressed
  if (digitalRead(buttonPin) == LOW) {
    // Reset brightness and previousMillis
    brightness = 0;
    previousMillis = currentMillis;
  }

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Gradually increase brightness
    brightness = brightness + fadeAmount;
    
    // Reverse the direction of fading at the ends of the fade
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }

    // Set the brightness of the LEDs
    analogWrite(led1, brightness);
    analogWrite(led2, brightness);
  }
}
