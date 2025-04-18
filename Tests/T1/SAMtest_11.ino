int buttonPin = 2;
int greenPins[] = {3, 5, 7, 9, 11, 13};  
int redPins[] = {2,4, 6, 8, 10, 12}; 
/*
pins used for Green: 2,4, 6, 8, 10, 12
-- Need to Re-confirm if time still allows--
-- Confirm if the button is working.. 
-- Use the Free wire mechanism to implement the switch --
pins used for Red : 3, 5, 7, 9, 11, 13
*/
bool buttonPressed = false;

void setup() {
  pinMode(buttonPin, INPUT);
  for (int i = 0; i < 6; i++) {
    pinMode(redPins[i], OUTPUT);
    pinMode(greenPins[i], OUTPUT);
  }
}

void loop() {
  buttonPressed = digitalRead(buttonPin) == HIGH;
  static unsigned long buttonTimer = 0;
  static bool sequenceOn = true;
  
  if (buttonPressed) {
    if (millis() - buttonTimer > 2000) {
      sequenceOn = false;  
      for (int i = 0; i < 6; i++) {
        analogWrite(redPins[i], 0);
        digitalWrite(greenPins[i], LOW);
      }
    }
  } else {
    buttonTimer = millis(); 
    sequenceOn = true; 
  }

  if (sequenceOn) {
    for (int i = 0; i < 6; i++) {
      for (int brightness = 0; brightness <= 255; brightness++) {
        analogWrite(redPins[i], brightness); 
        delay(10);
      }
      digitalWrite(greenPins[i], HIGH); 
      delay(400);
      digitalWrite(greenPins[i], LOW); 
    }
  }
  
// 	Need to use a function to reset the lights, 
 // So that everything can start from scratch.
  
}

