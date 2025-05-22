---
# Test 2 Practice

## DC Motor & H-Bridge

### 1. Rotate a DC Motor Forward and Reverse using H-Bridge (L298N)

**Explanation:**  
- You need two pins for direction (IN1, IN2), and optionally one for PWM (EN or Enable) for speed control.
- To go forward: IN1 HIGH, IN2 LOW.  
- To go reverse: IN1 LOW, IN2 HIGH.

**Code:**
```cpp
#define IN1 3
#define IN2 4
void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}
void loop() {
  // Forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(2000);
  // Reverse
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delay(2000);
}
```

---

### 2. Gradually Increase the Speed of a DC Motor (PWM) and Stop after 5 Seconds

**Explanation:**  
- Use PWM pin (e.g., ENA) and ramp from 0 to 255.
- After 5 seconds, set speed to 0 to stop the motor.

**Code:**
```cpp
#define ENA 5  // PWM pin
#define IN1 3
#define IN2 4
void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}
void loop() {
  for (int speed = 0; speed <= 255; speed += 5) {
    analogWrite(ENA, speed);
    delay(100);
  }
  delay(5000);
  analogWrite(ENA, 0); // Stop motor
  while(1); // Halt
}
```

---

## Ultrasonic Sensor

### 3. Function to Read Distance from HC-SR04 Ultrasonic Sensor

**Explanation:**  
- Trigger a pulse, measure echo time, convert to centimeters.

**Code:**
```cpp
#define TRIG 8
#define ECHO 9
long readDistanceCM() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  return duration * 0.034 / 2; // cm
}
void setup() {
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
}
```

---

### 4. Stop Motor if Object Detected Within 10 cm (with Ultrasonic)

**Code:**
```cpp
// Assume readDistanceCM() from above, ENA=5, IN1=3, IN2=4
void loop() {
  long distance = readDistanceCM();
  if (distance < 10) {
    analogWrite(ENA, 0); // Stop
  } else {
    analogWrite(ENA, 200); // Run
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  }
  delay(100);
}
```

---

## LCD Display

### 5. Display "Distance: xx cm" on a 16x2 LCD

**Explanation:**  
- Use LiquidCrystal library, wire LCD as required.

**Code:**
```cpp
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup() { lcd.begin(16, 2); }
void loop() {
  long d = readDistanceCM();
  lcd.setCursor(0,0);
  lcd.print("Distance: "); lcd.print(d); lcd.print(" cm");
  delay(500);
}
```

---

### 6. Show "Obstacle!" on Second Line if Distance < 10 cm

**Code:**
```cpp
void loop() {
  long d = readDistanceCM();
  lcd.setCursor(0,0);
  lcd.print("Distance: "); lcd.print(d); lcd.print(" cm   ");
  lcd.setCursor(0,1);
  if (d < 10) lcd.print("Obstacle!      ");
  else lcd.print("                ");
  delay(500);
}
```

---

## Interrupts

### 7. Toggle Motor ON/OFF with External Interrupt (Push Button)

**Explanation:**  
- Use `attachInterrupt` with a button on pin 2 (INT0).

**Code:**
```cpp
#define ENA 5
volatile bool motorOn = false;
void toggleMotor() { motorOn = !motorOn; }
void setup() {
  pinMode(ENA, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), toggleMotor, RISING);
}
void loop() {
  analogWrite(ENA, motorOn ? 200 : 0);
}
```

---

### 8. Display "Motor ON"/"Motor OFF" on LCD

**Code:**
```cpp
void loop() {
  analogWrite(ENA, motorOn ? 200 : 0);
  lcd.setCursor(0,0);
  lcd.print(motorOn ? "Motor ON " : "Motor OFF");
  delay(100);
}
```

---

## Strings

### 9. Read Sensor Value, Convert to String, Print on LCD

**Code:**
```cpp
int val = analogRead(A0);
String sval = String(val);
lcd.setCursor(0,0);
lcd.print(sval);
```

---

### 10. Combine String Message and Numeric Value, Print via Serial

**Code:**
```cpp
int val = analogRead(A0);
Serial.println("Sensor Value: " + String(val));
```

---

## Photoresistor

### 11. Read Photoresistor, Control LED Brightness

**Code:**
```cpp
int val = analogRead(A0);
int brightness = map(val, 0, 1023, 0, 255);
analogWrite(9, brightness); // LED on pin 9
```

---

### 12. Show Brightness Level on LCD in Percentage

**Code:**
```cpp
int percent = map(val, 0, 1023, 0, 100);
lcd.setCursor(0,1);
lcd.print("Light: "); lcd.print(percent); lcd.print("%   ");
```

---

## Photodiode

### 13. Detect Light Intensity, Turn Motor On if Below Threshold

**Code:**
```cpp
int light = analogRead(A0);
if (light < 200) analogWrite(ENA, 200); // Motor ON
else analogWrite(ENA, 0); // Motor OFF
```

---

## map() Function

### 14. Read Photoresistor (0–1023), Map to (0–255), Control Motor Speed

**Code:**
```cpp
int val = analogRead(A0);
int speed = map(val, 0, 1023, 0, 255);
analogWrite(ENA, speed);
```

---

### 15. Display Mapped Speed Value on LCD

**Code:**
```cpp
lcd.setCursor(0,1);
lcd.print("Speed: "); lcd.print(speed); lcd.print("     ");
```

---

# **Full Example: Multi-Feature Arduino Sketch**

```cpp name=full_example.ino
/*
Multi-feature code:
- Motor control (forward/reverse, speed)
- Ultrasonic stop
- LCD display
- Interrupt toggle
- Light sensor-to-motor mapping
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define ENA 6     // PWM for motor
#define IN1 7     // Motor direction
#define IN2 8
#define TRIG 9
#define ECHO 10
#define BUTTON 2  // Interrupt pin
#define LDR A0    // Photoresistor

volatile bool motorOn = false;

void toggleMotor() { motorOn = !motorOn; }

long readDistanceCM() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH, 30000); // timeout 30ms
  return duration * 0.034 / 2;
}

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  lcd.begin(16, 2);
  attachInterrupt(digitalPinToInterrupt(BUTTON), toggleMotor, FALLING);
  Serial.begin(9600);
}

void loop() {
  // Read sensors
  int ldrVal = analogRead(LDR);
  int speed = map(ldrVal, 0, 1023, 0, 255);
  long distance = readDistanceCM();

  // Motor logic: Stop if obstacle, else respond to light level and toggle
  if (distance < 10) {
    analogWrite(ENA, 0); // Stop for obstacle
  } else if (motorOn) {
    analogWrite(ENA, speed);
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); // Forward
  } else {
    analogWrite(ENA, 0); // Off
  }

  // LCD Display
  lcd.setCursor(0, 0);
  lcd.print("Dist:"); lcd.print(distance); lcd.print("cm   ");
  lcd.setCursor(0, 1);
  if (distance < 10)
    lcd.print("Obstacle!       ");
  else
    lcd.print("Speed: "); lcd.print(speed); lcd.print("   ");

  // Serial example
  Serial.println("LDR: " + String(ldrVal) + " Speed: " + String(speed));

  delay(500);
}
```
