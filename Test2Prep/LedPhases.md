---

# LED Phase Shift Demos

#### Demo for lighting up LEDs with phase differences

---

## 1. Lighting up 2 LEDs with a 60° Phase Shift (Section 11.7)

- Demonstrate how to create a phase shift between two LEDs using a potentiometer and Arduino.

---

### Circuit Link

[https://www.tinkercad.com/things/hWENEqbM2b5-117](https://www.tinkercad.com/things/hWENEqbM2b5-117)

---

## 2. How it Works

Two LEDs are controlled by PWM outputs. Both follow a cosine-based brightness curve determined by the potentiometer position, but one is shifted by 60°, causing it to lag behind the other and creating a clear phase difference.

---

## 3. Logic Table

| Pot Position (θ) | LED 1 (Pin 3) Output | LED 2 (Pin 5) Output | Phase Difference |
|------------------|----------------------|----------------------|------------------|
| 0°               | cos(0°) = 1 (max)    | cos(-60°) ≈ 0.5      | LED 2 lags 60°   |
| 60°              | cos(60°) ≈ 0.5       | cos(0°) = 1          | LED 2 catches up |
| 180°             | cos(180°) = -1       | cos(120°) = -0.5     | LED 2 brighter   |
| 360°             | cos(360°) = 1        | cos(300°) ≈ 0.5      | Cycle repeats    |

---

## 4. Wiring Diagram

```plaintext
Potentiometer wiper --> A5
LED 1 anode         --> Pin 3 (PWM)
LED 2 anode         --> Pin 5 (PWM)
GND                 --> LED cathodes, potentiometer GND
```

---

## 5. Example Code Snippet (Arduino/C++)

```cpp
int potIn;
int angle, intensity, inten_LED_1;
double angle_R;

void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(A5, INPUT);
}

void loop() {
  potIn = analogRead(A5);

  // LED 1: 0° to 360°
  angle = map(potIn, 4, 1023, 0, 360);
  angle_R = angle * PI / 180.0;
  intensity = 1000 * cos(angle_R);
  inten_LED_1 = map(intensity, -1000, 1000, 0, 255);
  analogWrite(3, inten_LED_1);

  // LED 2: -60° to 300° (60° phase shift)
  angle = map(potIn, 4, 1023, -60, 300);
  angle_R = angle * PI / 180.0;
  intensity = 1000 * cos(angle_R);
  inten_LED_1 = map(intensity, -1000, 1000, 0, 255);
  analogWrite(5, inten_LED_1);
}
```

---

## 6. Summary Table

| Step                 | What Happens                             |
|----------------------|-----------------------------------------|
| 1. Read potentiometer| Gets analog input (0–1023)              |
| 2. Map to angle      | For LED1: 0–360°, LED2: -60–300°        |
| 3. Compute cosine    | Produces -1000 to 1000 for both         |
| 4. Map to PWM        | Converts to 0–255 for LED brightness    |
| 5. Write to pins     | Lights up LEDs with 60° phase difference|


---

## 7. Notes

- For the second LED, both the minimum and maximum mapped angle are shifted by -60: `map(potIn, 4, 1023, -60, 300)`
- This shift is equivalent to a 60° phase lag.
- Both LEDs complete a full cosine cycle as the potentiometer sweeps from 0 to 1023.

---

### Key Changes for Phase Shift

- **Original LED (Pin 3):** Maps 0–1023 to 0–360°
- **Phase-Shifted LED (Pin 5):** Maps 0–1023 to -60–300°, lagging by 60°

---

## 8. Multiple LEDs Out of Phase (Section 11.8)

---

### Circuit Link

[https://www.tinkercad.com/things/6uVBjoeWuGc-118](https://www.tinkercad.com/things/6uVBjoeWuGc-118)

---

## 9. Example Code: Multiple LEDs with Phase Shift

```cpp
int potIn;
int angle_D, intensity, inten_LED_1;
int pinNum[] = {3, 5, 6, 9, 10, 11};

void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(A5, INPUT);
}

void loop() {
  potIn = analogRead(A5);

  for (int led_num = 0; led_num < 6; led_num++) {
    int new_angle = led_num * 60;  // Phase shift each LED by 60°
    int angle_D = map(potIn, 4, 1023, 0 + new_angle, 360 + new_angle);
    switchLED(led_num, angle_D);
  }
}

void switchLED(int led_num, int angleInDegrees) {
  double angle_R;
  int intensity, inten_LED_1;

  angle_R = angleInDegrees * PI / 180.0;
  intensity = 1000 * cos(angle_R);
  inten_LED_1 = map(intensity, -1000, 1000, 0, 255);
  analogWrite(pinNum[led_num], inten_LED_1);
}
```

---

### How It Works

- `switchLED()` handles cosine calculation and mapping for a given LED and phase.
- Each LED is assigned a phase offset of 60° relative to the previous.
- This creates a rotating effect if the LEDs are arranged in a circle.

---

## 10. Summary Table

| LED Number | Phase Shift (°) |
|------------|-----------------|
| 0          | 0               |
| 1          | 60              |
| 2          | 120             |
| 3          | 180             |
| 4          | 240             |
| 5          | 300             |

---

## 11. References

- [Tinkercad circuit for 2 LEDs phase shift](https://www.tinkercad.com/things/hWENEqbM2b5-117)
- [Tinkercad circuit for multiple LEDs out of phase](https://www.tinkercad.com/things/6uVBjoeWuGc-118)

---
