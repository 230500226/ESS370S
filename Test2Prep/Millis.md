---

# Arduino Timing with `millis()`: Non-Blocking Delays, Math, and Phase Shifts

#### Demos for using `millis()` for timing, arithmetic, PWM effects, and phase-shifted LED control.

---

## 1. What is `millis()`? (Section 6.1)

---

### `millis()` Definition

- **Returns:** Number of milliseconds since the Arduino board was powered on or reset.
- **Type:** `unsigned long` (32-bit, non-negative).
- **Hardware-based:** Keeps running in the background.
- **Purpose:** Provides precise timing without blocking code execution.

**Syntax:**
```cpp
unsigned long currentTime = millis(); // Returns elapsed time in ms
```

**Typical Usage (Non-blocking Delay):**
```cpp
if (millis() - previousTime > 1000) {
  // Action every 1 second
  previousTime = millis();
}
```

- Good for tracking elapsed time without stopping program flow.

---

### Circuit Link

[Circuit design Design 6.1 - Tinkercad](https://www.tinkercad.com/things/kpAb7CrgImn-design-61/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard%2Fdesigns%2Fcircuits)

---

## 2. Using millis() to switch on after time elapsed (Section 6.2)

---

[Circuit link](https://www.tinkercad.com/things/hdsabCizgaF-design-62)

---

## 3. Using Division and Modulus with millis() (Sections 6.3.1, 6.3.2)

---

### Circuit Links

6.3.1: [https://www.tinkercad.com/things/lKIDhkCIZtz-design-631](https://www.tinkercad.com/things/lKIDhkCIZtz-design-631)  
6.3.2: [https://www.tinkercad.com/things/4A2Abbpw89f-design-632](https://www.tinkercad.com/things/4A2Abbpw89f-design-632)

---

**Concepts Demonstrated:**

1. **Elapsed Time:**  
   - `millis()` tracks system uptime in milliseconds.
   - Used to check for time intervals (e.g., every 1000ms).

2. **Conditionals:**  
   - `if(value1 >= 1000)` triggers actions after elapsed time.
   - Non-blocking, so math operations continue in the background.

3. **Arithmetic Operations:**  
   - Division and modulus (`%`) used to create cyclic or periodic behavior.
   - Results are streamed to Serial Monitor for real-time feedback.

---

**Key Program Flow:**

- Track elapsed time.
- Trigger LED or calculations at set intervals.
- Perform live division/modulus math.
- Print results to Serial Monitor.
- All without blocking execution (unlike `delay()`).

---

## 4. LED Fading and PWM with Modulus (Sections 6.3.3, 6.4)

---

6.3.3: [https://www.tinkercad.com/things/9zITF5GBAU6-design-633](https://www.tinkercad.com/things/9zITF5GBAU6-design-633)  
6.4: [https://www.tinkercad.com/things/aLAlJyfF8Cv-design-64](https://www.tinkercad.com/things/aLAlJyfF8Cv-design-64)

---

### Key Concepts

- Use `analogWrite(3, result);` to set LED brightness.
- Maximum PWM value is 255 (8-bit range).
- `result % 256` ensures the output cycles smoothly from 0 to 255 and wraps around.
- Using division slows the fade cycle.

**Example:**
```cpp
unsigned long value1 = millis();
int value2 = 256;
int result = value1 % value2;
analogWrite(3, result);
```

---

## 5. Speeding Up Intensity Change (Section 6.5)

---

[Circuit link](https://www.tinkercad.com/things/ikMWD3E9gSY-design-65)

- Set `value2 = 10;`
- `result = value1 / value2;`
- This increases the rate at which the PWM value reaches 255, making the LED fade much faster.

---

## 6. On-board LED Flashing (Section 6.6)

---

[Circuit link](https://www.tinkercad.com/things/ft3BUaDXnx9-design-66)

---

## 7. Phase Shift Concepts with Arrays & Functions (Sections 6.7.1, 6.7.2)

---

6.7.1: [https://www.tinkercad.com/things/ft3BUaDXnx9-design-671](https://www.tinkercad.com/things/ft3BUaDXnx9-design-671)  
6.7.2: [https://www.tinkercad.com/things/fza50O088jA-design-672](https://www.tinkercad.com/things/fza50O088jA-design-672)

### Concepts

- Arrays (`pinNum[]`) store pin numbers for controlling multiple LEDs.
- Functions (e.g., `LED_brightness()`) encapsulate and reuse logic.
- Phase shifting is achieved by offsetting the calculation for each LED.
- Modulo operations keep brightness in the valid PWM range.
- Conditional checks (`if(result<128)`) allow for threshold-based behaviors.

**Phase Wrapping Example:**
```cpp
if (intensity > 255) {
  intensity = intensity - 256;
}
```
- Ensures intensity cycles through 0–255, never exceeding PWM limits.

---

## 8. Adding a Third LED (Section 6.8)

---

[Circuit link](https://www.tinkercad.com/things/4P7qAsTVquU-design-68)

- Use functions to set different brightness levels for each LED at different phases.
- Demonstrates coordination of multiple LEDs using time, math, and phase logic.

---

## Summary Table

| Section   | Concept                       | Key Function(s) / Concept        | Circuit Example                   |
|-----------|-------------------------------|----------------------------------|-----------------------------------|
| 6.1       | millis() intro                | `millis()`                       | [6.1](https://www.tinkercad.com/things/kpAb7CrgImn-design-61) |
| 6.2       | Switch after delay            | Time check, non-blocking         | [6.2](https://www.tinkercad.com/things/hdsabCizgaF-design-62) |
| 6.3.1/2   | Division & modulus            | `/`, `%`, Serial output          | [6.3.1](https://www.tinkercad.com/things/lKIDhkCIZtz-design-631), [6.3.2](https://www.tinkercad.com/things/4A2Abbpw89f-design-632) |
| 6.3.3/6.4 | LED fading & PWM              | `analogWrite()`, `% 256`         | [6.3.3](https://www.tinkercad.com/things/9zITF5GBAU6-design-633), [6.4](https://www.tinkercad.com/things/aLAlJyfF8Cv-design-64) |
| 6.5       | Faster intensity change       | Smaller division value           | [6.5](https://www.tinkercad.com/things/ikMWD3E9gSY-design-65) |
| 6.6       | On-board LED flashing         | millis(), toggling output        | [6.6](https://www.tinkercad.com/things/ft3BUaDXnx9-design-66) |
| 6.7.1/2   | Phase shift, arrays, wrapping | Arrays, modulo, functions, phase | [6.7.1](https://www.tinkercad.com/things/ft3BUaDXnx9-design-671), [6.7.2](https://www.tinkercad.com/things/fza50O088jA-design-672) |
| 6.8       | Three LEDs, phased            | Arrays, phase, functions         | [6.8](https://www.tinkercad.com/things/4P7qAsTVquU-design-68) |

---

**Key Concepts:**
- Use `millis()` for non-blocking timing and real-time math.
- Modulus (%) and division (/) create cycles and periodic patterns.
- Arrays and functions help manage multiple LEDs and repetitive logic.
- Phase shifting and modulo wrapping enable complex LED effects.

---
