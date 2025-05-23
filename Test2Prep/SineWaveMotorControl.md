---

# Sine Wave Motor Controller: DC Motor Oscillation with L293D

#### Demo and code for creating a DC motor oscillation system with user-adjustable amplitude and frequency, direction switching, and serial commands.

---

## 1. Hardware Connections

| Function      | Arduino Pin | Description                              |
|---------------|-------------|------------------------------------------|
| PWM (speed)   | 3           | Controls motor speed (to L293D EN1/IN1)  |
| Direction     | 2           | Controls direction (to L293D IN2)        |
| Potentiometer | A0          | Sets maximum amplitude (user knob)       |
| L293D         | ...         | Use standard wiring for power, GND, etc. |

---

## 2. Features & Requirements

- **Motor speed**: Varies according to a sine wave
- **Direction switching**: Changes when sine wave crosses zero
- **Amplitude**: Set by potentiometer (A0)
- **Frequency**: User adjustable via serial command `"freq(value)"`
- **Serial command**: Also supports `"mul(value1,value2)"` for multiplication calculations

---

## 3. Example Code

```cpp
// Sine wave motor controller for oscillating motion

#define PWM_PIN      3   // PWM output to L293D EN1
#define DIR_PIN      2   // Direction control to L293D IN2
#define POT_PIN      A0  // Potentiometer for amplitude

float frequency = 0.5;   // Hz, default oscillation frequency
float amplitude = 255;   // Max PWM value (0-255)
unsigned long lastUpdate = 0;
const float PI = 3.14159265;
int lastSineSign = 1;    // Detect zero crossing for direction change
String serialBuffer = "";

void setup() {
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
  Serial.begin(9600);
  // Start with motor stopped
  analogWrite(PWM_PIN, 0);
  digitalWrite(DIR_PIN, LOW);
}

void loop() {
  // 1. Read potentiometer for amplitude scaling
  int potValue = analogRead(POT_PIN);
  amplitude = map(potValue, 0, 1023, 0, 255);

  // 2. Calculate sine wave based on millis() and frequency
  unsigned long now = millis();
  float t = now / 1000.0; // seconds
  float angle = 2 * PI * frequency * t;
  float sineVal = sin(angle); // Range: -1 to 1

  // 3. Detect zero crossing for direction switch
  int sineSign = (sineVal >= 0) ? 1 : -1;
  static int lastDir = 0;
  if (sineSign != lastSineSign) {
    // Change direction when crossing zero
    lastSineSign = sineSign;
    lastDir = !lastDir;
    digitalWrite(DIR_PIN, lastDir); // Toggle direction pin
  }

  // 4. Set speed (PWM proportional to abs(sineVal) * amplitude)
  int speedVal = abs(sineVal) * amplitude;
  analogWrite(PWM_PIN, speedVal);

  // 5. Handle serial commands for frequency and multiplication
  handleSerialCommands();

  // Small delay for loop stability
  delay(10);
}

void handleSerialCommands() {
  // Append incoming serial bytes to a buffer
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (serialBuffer.length() > 0) {
        serialBuffer.trim();
        processCommand(serialBuffer);
        serialBuffer = "";
      }
    } else {
      serialBuffer += c;
    }
  }
}

void processCommand(String cmd) {
  cmd.trim();
  if (cmd.startsWith("freq(")) {
    int start = cmd.indexOf('(') + 1;
    int end = cmd.indexOf(')');
    if (start > 0 && end > start) {
      String valStr = cmd.substring(start, end);
      float val = valStr.toFloat();
      if (val > 0) {
        frequency = val;
        Serial.print("Frequency set to: ");
        Serial.println(frequency);
      } else {
        Serial.println("Invalid frequency value.");
      }
    }
  } else if (cmd.startsWith("mul(")) {
    int start = cmd.indexOf('(') + 1;
    int comma = cmd.indexOf(',', start);
    int end = cmd.indexOf(')', comma);
    if (start > 0 && comma > start && end > comma) {
      String val1 = cmd.substring(start, comma);
      String val2 = cmd.substring(comma + 1, end);
      float num1 = val1.toFloat();
      float num2 = val2.toFloat();
      float result = num1 * num2;
      Serial.print("mul("); Serial.print(num1); Serial.print(","); Serial.print(num2); Serial.print(") = ");
      Serial.println(result);
    } else {
      Serial.println("Invalid mul() format. Use: mul(value1,value2)");
    }
  } else {
    Serial.println("Unknown command.");
  }
}
```

---

## 4. Notes

- **Sine wave modulation:** `sin(angle)` sets speed, direction changes at zero crossing.
- **Amplitude (max speed):** Controlled by potentiometer (0–255).
- **Frequency:** Set via serial, e.g. `freq(2.0)` for 2 Hz.
- **Multiplication command:** Send `mul(4,5)` via serial for on-the-fly calculation.
- **Direction:** Changes every half cycle (when sine crosses 0).

---

## 5. Summary Table

| Feature             | How it's Controlled              |
|---------------------|----------------------------------|
| Speed (amplitude)   | Potentiometer (A0)              |
| Frequency           | Serial command `freq(value)`     |
| Direction           | Sine wave zero crossing          |
| Multiplication      | Serial command `mul(val1,val2)`  |

---

## 6. References

- [L293D Motor Driver Guide](https://www.ti.com/lit/ds/symlink/l293d.pdf)
- [Arduino L293D Tutorial](https://docs.arduino.cc/tutorials/arduino-motor-shield-rev3/)
- [Arduino Sine Wave Generation](https://www.arduino.cc/reference/en/language/functions/math/trigonometry/sin/)

---
