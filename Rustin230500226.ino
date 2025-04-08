[ldxt460s@ThinkpadT460sldxt460s temp]$ cat Rustin230500226.ino 
const int staticNumLeds = 6;
const int dynNumLeds = 6;
int staticLedPins[staticNumLeds] = {2,4,7,8,12,13};
int dynLedPins[dynNumLeds] = {3,5,6,9,10,11};

unsigned long previousMillis1 = 0;

// Led brightness foreach
int brightness[dynNumLeds] = {0, 42.5, 85, 127.5, 170, 212.5};

int fadeAmount = 10; // Amount to change the brightness

void setup() {
  // put your setup code here, to run once:

  // Green LEDS
  for (int i = 0; i < staticNumLeds; i++){
    pinMode(staticLedPins[i], OUTPUT);
  }
  // Red LEDS
  for (int i = 0; i < dynNumLeds; i++){
    pinMode(dynLedPins[i], OUTPUT);
  }
  pinMode(A5, INPUT);
}

void loop() {

  unsigned long currentMillis = millis();
  int interval = 30;

  // Update LED 1
  if (currentMillis - previousMillis1 >= interval) {
    previousMillis1 = currentMillis;

  for (int i = 0; i <= dynNumLeds; i++){
      brightness[i] = brightness[i] + fadeAmount;

      if (brightness[i] == 255) {
        brightness[i] = 0;
        digitalWrite(staticLedPins[i], HIGH);
      } else {
        if (currentMillis - previousMillis1 >= 400){
          digitalWrite(staticLedPins[i], LOW);
        }

      }
      analogWrite(dynLedPins[i], brightness[i]);
    }
    delay(300);
  }
}
