---

# Arduino Interrupts: Practical Usage and Examples

#### Demo for handling time-sensitive events with Arduino interrupts

---

## 1. What are Interrupts?

Interrupts in Arduino allow you to pause the normal execution of your program to handle time-sensitive events. When an interrupt occurs, the processor stops what it's doing, executes a special function called an Interrupt Service Routine (ISR), then returns to what it was doing before.

---

## 2. Key Interrupt Functions

| Function                                | Purpose                                        |
|------------------------------------------|------------------------------------------------|
| `attachInterrupt()`                      | Enable a hardware interrupt on a given pin     |
| `detachInterrupt()`                      | Disable a specific hardware interrupt          |
| `interrupts()` / `noInterrupts()`        | Globally enable or disable all interrupts      |

### 2.1. attachInterrupt()

```cpp
attachInterrupt(digitalPinToInterrupt(pin), ISR, mode);
```
- `digitalPinToInterrupt(pin)`: Converts a pin number to the interrupt number.
- `ISR`: Function to call (must be fast, no delay or Serial if possible).
- `mode`: When to trigger (`LOW`, `CHANGE`, `RISING`, `FALLING`).

### 2.2. detachInterrupt()

```cpp
detachInterrupt(digitalPinToInterrupt(pin));
```
- Disables the interrupt for the given pin.

### 2.3. interrupts() / noInterrupts()

```cpp
interrupts();    // Enable all interrupts
noInterrupts();  // Disable all interrupts
```

---

## 3. Pin/Interrupt Mapping (UNO)

| Interrupt Number | Arduino Pin |
|------------------|-------------|
| 0                | 2           |
| 1                | 3           |

Use `digitalPinToInterrupt(pin)` for portability across boards.

---

## 4. Example: Turn on an LED using Interrupt (Section 10.1)

### Circuit Link

[https://www.tinkercad.com/things/fX6nKlKbS4V-design-101](https://www.tinkercad.com/things/fX6nKlKbS4V-design-101)

---

### Example Code

```cpp
void setup() {
  attachInterrupt(0, function_1, RISING); // Interrupt 0 = Pin 2, trigger on rising edge
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  pinMode(4, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}

void function_1() {
  Serial.println("Button 1 pressed");
  digitalWrite(4, HIGH);
}
```

---

### Notes

- The interrupt triggers when Pin 2 goes from LOW to HIGH (`RISING`).
- The main loop blinks the built-in LED as a "heartbeat" indicator:
  - Shows the main program is running.
  - Proves that interrupts don't block the main loop or delay handling.
- The ISR (`function_1`) turns on another LED and prints to Serial.

---

## 5. Example: Toggle and Switch Off a Second LED with Two Interrupts (Section 10.2)

### Circuit Link

[https://www.tinkercad.com/things/jPvIGZR9bDR-design-102](https://www.tinkercad.com/things/jPvIGZR9bDR-design-102)

---

### Example Code

```cpp
bool previousOn;
void setup() {
  attachInterrupt(0, function_1, RISING);       // Pin 2
  attachInterrupt(1, swithOffLED, RISING);      // Pin 3
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  pinMode(4, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}

void function_1() {
  previousOn = !previousOn;
  digitalWrite(4, previousOn);
  Serial.println("Button 1 pressed");
}

void swithOffLED() {
  digitalWrite(4, LOW);
  Serial.println("Button 2 pressed");
}
```

---

### Notes

- **Interrupt 0** is hardwired to **Digital Pin 2** on Arduino UNO.
- **Interrupt 1** is hardwired to **Digital Pin 3**.
- `attachInterrupt(0, ...)` and `attachInterrupt(1, ...)` use interrupt numbers, but you can also use `attachInterrupt(digitalPinToInterrupt(pin), ...)` for clarity.

---

### How the Hardware Detects Rising Edge

- The chip monitors the voltage on the interrupt pins.
- When voltage transitions from LOW to HIGH (rising edge), it triggers the ISR.
- The processor jumps to the ISR, executes it, and then resumes the main code.

---

### Summary Table

| Step                    | What Happens                                    |
|-------------------------|-------------------------------------------------|
| 1. Set up interrupts    | Use `attachInterrupt()` for each button action  |
| 2. Main loop            | Continues running, unaffected by interrupts     |
| 3. Press button         | ISR is called, LED toggles or switches off      |
| 4. Heartbeat LED        | Shows main code is still running                |

---

## 6. References

- [Arduino attachInterrupt() Documentation](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)
- [Tinkercad circuit for 10.1](https://www.tinkercad.com/things/fX6nKlKbS4V-design-101)
- [Tinkercad circuit for 10.2](https://www.tinkercad.com/things/jPvIGZR9bDR-design-102)

---
