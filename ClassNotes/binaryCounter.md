# Binary Counter with Arduino, Button, and 5 LEDs

This guide explains how to build a binary counter using an Arduino, a push-button, and 5 LEDs.

## Components Needed

- Arduino UNO (or similar)
- 5 LEDs
- 5 current-limiting resistors (220–330Ω)
- 1 push button
- 1 pull-down resistor for button (if not using internal pullup)
- Jumper wires
- Breadboard

## Circuit Diagram

- Connect each LED (and a resistor) to Arduino pins 2–6, and the other end to ground.
- Connect one side of the button to pin 7, the other side to ground.

## Step-by-Step Instructions

### 1. Define Pins

Set up arrays for LED pins and the button pin:

```cpp
const int ledPins[5] = {2, 3, 4, 5, 6};
const int buttonPin = 7;
```

### 2. Initialize Counter and Debounce Variables

```cpp
int counter = 0;
bool lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;
```

### 3. Setup Pin Modes

In `setup()`, set LED pins as outputs and button pin as input with pullup:

```cpp
void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(buttonPin, INPUT_PULLUP);
}
```

### 4. Update LEDs to Display Counter Value

```cpp
void updateLEDs(int value) {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPins[i], (value >> i) & 0x01);
  }
}
```

### 5. Detect Button Press and Increment Counter

Debounce the button and increment the counter when pressed:

```cpp
void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && lastButtonState == HIGH) {
      counter = (counter + 1) % 32; // 5 bits, wraps at 32
      updateLEDs(counter);
    }
  }
  lastButtonState = reading;
}
```

### 6. Upload and Test

- Upload the code to your Arduino.
- Each press of the button increments the counter.
- LEDs display the counter value in binary (LED0 = LSB, LED4 = MSB).

## Full Code

```cpp
const int ledPins[5] = {2, 3, 4, 5, 6}; // LEDs connected to pins 2-6
const int buttonPin = 7;                // Button connected to pin 7

int counter = 0;                        // Binary counter (0-31)
bool lastButtonState = LOW;             // Previous button state
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // Debounce time in milliseconds

void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);        // Set LED pins to output
  }
  pinMode(buttonPin, INPUT_PULLUP);     // Set button pin to input with pullup
  updateLEDs(counter);                  // Initialize LEDs
}

void updateLEDs(int value) {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPins[i], (value >> i) & 0x01);
  }
}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && lastButtonState == HIGH) {
      counter = (counter + 1) % 32; // Increment and wrap at 32
      updateLEDs(counter);
    }
  }
  lastButtonState = reading;
}
```

---

### How Bitshift and Masking Work in the Binary Counter

In the binary counter code, each LED represents one bit of a binary number. The counter variable holds a decimal value, and we use bitwise operations to extract each bit for display.

**Key code:**
```cpp
for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], (counter >> i) & 1);
}
```

**Explanation:**
- `counter >> i`: This shifts the bits of the counter right by `i` positions, moving the bit of interest to the least significant position.
- `& 1`: This masks all bits except the least significant one, effectively isolating the value of the i-th bit.
- The result (0 or 1) turns each LED on or off accordingly.

**Example (counter = 13, binary 01101):**

| i | counter >> i | Binary | & 1 | LED State |
|---|--------------|--------|-----|-----------|
| 0 | 13 >> 0 = 13 | 01101  | 1   | ON        |
| 1 | 13 >> 1 = 6  | 00110  | 0   | OFF       |
| 2 | 13 >> 2 = 3  | 00011  | 1   | ON        |
| 3 | 13 >> 3 = 1  | 00001  | 1   | ON        |
| 4 | 13 >> 4 = 0  | 00000  | 0   | OFF       |

This technique lets the program convert a decimal counter value to its binary representation, lighting up LEDs to visually show the counter.

