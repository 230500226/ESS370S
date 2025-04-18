Parkding sensor

```ino
// init defs
const int TRIG_PIN = 3;  // triggger Pin
const int ECHO_PIN = 2;  // echo Pin
const int GREEN_LED = 4; // green
const int ORANGE_LED = 5; // orange 🍊
const int RED_LED = 6;   // red

long duration = 0;
int distance = 0;

void setup()
{
  //pin config
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(ORANGE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  Serial.begin(9600); //init serial coms >> cat distance
}

void loop()
{
  //tx trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  //rx echo 
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.0343 / 2;

  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.println(" cm");

  //distance parking sensor assist logic
  if (distance > 340)
  {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(ORANGE_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }
  else if (distance >= 210)
  {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(ORANGE_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }
  else if (distance >= 100)
  {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(ORANGE_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }
  else
  {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(ORANGE_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }

  delay(100); //reduce duty cycle for incr stabilty
}
```

Better parking sensor (increased accuracy due to using data sheet timings and no interupts decreasing overhead)

```ino
// Pin definitions
const int TRIG_PIN = 3;     // Trigger Pin
const int ECHO_PIN = 2;     // Echo Pin
const int GREEN_LED = 4;    // Green LED
const int ORANGE_LED = 5;   // Orange LED
const int RED_LED = 6;      // Red LED

long duration = 0;
int distance = 0;

void setup()
{
  // Configure pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(ORANGE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  Serial.begin(9600); // Initialize Serial communication
}

void loop()
{
  measureDistance(); // Measure distance using ultrasonic sensor

  if (distance == -1) {
    // No valid distance reading, turn all LEDs off
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(ORANGE_LED, LOW);
    digitalWrite(RED_LED, LOW);
  } else {
    // LED logic based on distance
    if (distance > 340) {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(ORANGE_LED, LOW);
      digitalWrite(RED_LED, LOW);
    } else if (distance >= 210) {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(ORANGE_LED, LOW);
      digitalWrite(RED_LED, LOW);
    } else if (distance >= 100) {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(ORANGE_LED, HIGH);
      digitalWrite(RED_LED, LOW);
    } else {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(ORANGE_LED, LOW);
      digitalWrite(RED_LED, HIGH);
    }
  }

  delay(100); // Small delay for stability
}

void measureDistance()
{
  // Send 10 µs trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo duration with timeout
  noInterrupts();
  duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout (~5m distance)
  interrupts();

  if (duration == 0) {
    Serial.println("No echo received");
    distance = -1; // Invalid distance
    return;
  }

  // Calculate distance in cm
  distance = duration * 0.0343 / 2;

  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.println(" cm");
}
```
