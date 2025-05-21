Sure! Here’s a breakdown for the L293D motor driver and how to control one motor with it, following the same structure as your Ultrasonic Sensor notes:

---

# L293D Motor Driver

#### Demo for controlling a DC motor - TODO

---

## 1. Pinout

- **VCC1 (Pin 16):** Logic Voltage (+5V)
- **VCC2 (Pin 8):** Motor Voltage (+5V to +12V)
- **GND (Pins 4, 5, 12, 13):** Ground
- **IN1/IN2 (Pins 2, 7):** Input pins for Motor A direction control
- **OUT1/OUT2 (Pins 3, 6):** Output pins for Motor A
- **EN1 (Pin 1):** Enable pin for Motor A (must be HIGH to run)
- (Similarly, IN3/IN4, OUT3/OUT4, EN2 for Motor B)

---

## 2. How it Works

The L293D is a dual H-Bridge motor driver IC. To control one DC motor:

1. **Set EN1 HIGH:**  
   EN1 (Enable 1, Pin 1) must be HIGH to activate Motor A outputs.

2. **Control Direction:**
   - IN1 HIGH, IN2 LOW: Motor turns one direction.
   - IN1 LOW, IN2 HIGH: Motor turns the opposite direction.
   - IN1 LOW, IN2 LOW or both HIGH: Motor stops.

3. **Speed Control:**  
   You can use PWM on EN1 to control speed.
   Or sir way here

---

## 3. Logic Table

| EN1 | IN1 | IN2 | OUT1 | OUT2 | Motor Action     |
|-----|-----|-----|------|------|-----------------|
|  1  |  1  |  0  |  1   |  0   | Forward         |
|  1  |  0  |  1  |  0   |  1   | Reverse         |
|  1  |  0  |  0  |  0   |  0   | Stop (Coast)    |
|  1  |  1  |  1  |  1   |  1   | Stop (Brake)    |
|  0  |  X  |  X  |  Z   |  Z   | Disabled        |

- 1 = HIGH, 0 = LOW, X = Don’t care, Z = High impedance (outputs off)

---

## 4. Wiring Diagram

```plaintext
   Microcontroller      L293D         Motor
   --------------     -------     -----------
   Digital Pin 1  -->  IN1  |     | OUT1 ----+
   Digital Pin 2  -->  IN2  |     | OUT2 ----+
   PWM Pin       -->  EN1  |
   +5V           -->  VCC1 |
   +9V or 12V    -->  VCC2 |
   GND           -->  GND  |
```

---

## 5. Example Code Snippet (Arduino/C++)

```cpp
#define EN1 9   // PWM pin
#define IN1 8
#define IN2 7

void setup() {
  pinMode(EN1, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop() {
  // Forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(EN1, 200);   // Speed (0-255)

  delay(2000);

  // Stop
  analogWrite(EN1, 0);

  delay(1000);

  // Reverse
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(EN1, 150);

  delay(2000);

  // Stop
  analogWrite(EN1, 0);
  delay(1000);
}
```

- `analogWrite(EN1, value)` controls speed (0=stop, 255=full speed).
- Changing IN1/IN2 controls direction.

---

## 6. Summary Table

| Step               | What Happens                         |
|--------------------|-------------------------------------|
| 1. Enable EN1      | Activates Motor A outputs           |
| 2. Set IN1/IN2     | Determines motor direction          |
| 3. PWM on EN1      | Controls speed                      |
| 4. Vary IN1/IN2    | Change direction/stop motor         |

---

## 7. Sirs way of controlling speed

Control speed by setting EN1 (Enable) HIGH and using PWM directly on IN1 or IN2.

**Pin Connections:**

| L293D Pin | Connect To         | Purpose             |
|-----------|--------------------|---------------------|
| EN1       | 5V (HIGH)          | Always enabled      |
| IN1       | PWM pin (e.g., 9)  | Fwd speed control   |
| IN2       | PWM pin (e.g., 10) | Rev speed control   |

- Only one of IN1/IN2 receives PWM at a time; the other is LOW.
- IN1 PWM + IN2 LOW: Forward (speed by PWM value)
- IN1 LOW + IN2 PWM: Reverse (speed by PWM value)
- No PWM on both: Motor stops

**Example code**

```cpp
#define EN1  9    // Enable pin (always HIGH)
#define IN1  6    // PWM-capable pin for forward
#define IN2  5    // PWM-capable pin for reverse

void setup() {
  pinMode(EN1, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(EN1, HIGH); // Always enabled
}

void loop() {
  // Forward at variable speed
  analogWrite(IN1, 180);   // 0-255 for speed
  digitalWrite(IN2, LOW);
  delay(2000);

  // Stop
  analogWrite(IN1, 0);
  delay(1000);

  // Reverse at variable speed
  digitalWrite(IN1, LOW);
  analogWrite(IN2, 180);
  delay(2000);

  // Stop
  analogWrite(IN2, 0);
  delay(1000);
}
```
---

## 8. References

- [L293D Datasheet (TI)](https://www.ti.com/lit/ds/symlink/l293d.pdf)
- [Arduino L293D Motor Shield Tutorial](https://docs.arduino.cc/tutorials/arduino-motor-shield-rev3/)

---
