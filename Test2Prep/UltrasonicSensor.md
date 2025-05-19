# HC-SR04

## [TinkerCad](https://www.tinkercad.com/things/fWnMB380ROK-hc-sr04parkingsensor)

---

## 1. Pinout

- **VCC**: Power (+5V)
- **Trig**: Trigger Input Pin
- **Echo**: Echo Output Pin
- **GND**: Ground

---

## 2. How it Works

The sensor operates in these steps:

1. **Trigger Pulse:**  
   Send a 10-microsecond HIGH pulse to the TRIG pin to start measurement.

2. **Sound Emission:**  
   The sensor emits an 8-cycle 40kHz ultrasonic burst.

3. **Echo Reception:**  
   The ECHO pin goes HIGH. It stays HIGH until the ultrasonic burst reflects off an object and returns.

4. **Time Measurement:**  
   The length of the HIGH pulse on ECHO represents the time (in microseconds, μs) for the sound to go to the object and back.

---

## 3. Timing Diagram

```plaintext
Trigger Pin:   ________|‾‾‾‾‾‾‾‾|___________   (10 μs pulse)
Echo Pin:      ________|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|___   (Time = t μs)
```

---

## 4. Calculating Distance

**The main formula:**

Distance to object (in centimeters):

```
distance_cm = (duration * speed_of_sound) / 2
```

- `duration`: Time in microseconds (μs) the ECHO pin is HIGH.
- `speed_of_sound`: 0.0343 cm/μs (343 m/s = 0.0343 cm/μs).
- We divide by 2 because the time measured is for the round trip (to the object and back).

**Scientific Notation Explanation:**

- Speed of sound in air at 20°C ≈ 343 m/s = 34300 cm/s
- 1 second = 1,000,000 microseconds (μs)
- So, speed in cm/μs:
  ```
  34300 cm/s ÷ 1,000,000 μs/s = 0.0343 cm/μs
  ```
  Or, in scientific notation:  
  \(3.43 \times 10^4 \) cm/s  
  \(1 \times 10^6 \) μs/s  
  So,
  \[
  \text{Speed in cm/μs} = \frac{3.43 \times 10^4}{1 \times 10^6} = 3.43 \times 10^{-2} = 0.0343 \text{ cm/μs}
  \]

**Distance equation:**

\[
\text{distance (cm)} = \frac{\text{duration (μs)} \times 0.0343 \text{ cm/μs}}{2}
\]

---

## 5. Example Code Snippet

```cpp
long duration;
int distance;

digitalWrite(TRIG_PIN, LOW);
delayMicroseconds(2);
digitalWrite(TRIG_PIN, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG_PIN, LOW);

duration = pulseIn(ECHO_PIN, HIGH);
distance = duration * 0.0343 / 2; // Calculate distance in cm
```

- `pulseIn(ECHO_PIN, HIGH)` measures the time (in μs) that the ECHO pin is HIGH.
- The multiplication by 0.0343 converts microseconds to centimeters.
- Division by 2 accounts for the round trip.

---

## 6. Summary Table

| Step            | What Happens                                    |
|-----------------|------------------------------------------------|
| 1. Trigger      | 10μs pulse to TRIG                             |
| 2. Send Pulse   | Sensor emits 8x 40kHz pulse                    |
| 3. Wait Echo    | ECHO pin HIGH while waiting for return signal  |
| 4. Measure Time | Time ECHO is HIGH (duration) = round trip time |
| 5. Calculate    | distance = (duration × 0.0343) / 2 (in cm)     |

---

## 7. References

- [HC-SR04 Datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf)
- [Speed of Sound (Wikipedia)](https://en.wikipedia.org/wiki/Speed_of_sound)

---
