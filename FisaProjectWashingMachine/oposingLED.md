# Cross-Opposing LED Drive

---

## 1. LED Wiring: Cross-Opposing Configuration

The four LEDs are connected between the pins as follows (each LED is between a unique pair of pins):

| LED  | Anode (A) | Cathode (C) |
|------|-----------|-------------|
| LED1 | 5         | 4           |
| LED2 | 7         | 6           |
| LED3 | 4         | 5           |
| LED4 | 6         | 7           |

**How this works:**  
- Each LED is placed between two pins, with opposite polarity for the opposing LED in the pair.
- To light up an LED, set one pin HIGH and the other LOW.  
- To turn it off or avoid reverse-bias, set both pins LOW (or both HIGH).

> **Note:** No two LEDs are between the same two pins with the same polarity.

---

## 2. Code: The Drum LED Sequence Table

The code uses an array to represent 8 steps in the LED animation (for smooth movement):

```cpp
const int DRUM_PINS[4] = {4, 5, 6, 7};

const int DRUM_SEQ[8][4] = {
  {HIGH, LOW, LOW, LOW},   // Step 0
  {HIGH, LOW, HIGH, LOW},  // Step 1
  {LOW, LOW, HIGH, LOW},   // Step 2
  {LOW, HIGH, HIGH, LOW},  // Step 3
  {LOW, HIGH, LOW, LOW},   // Step 4
  {LOW, HIGH, LOW, HIGH},  // Step 5
  {LOW, LOW, LOW, HIGH},   // Step 6
  {HIGH, LOW, LOW, HIGH}   // Step 7
};
```

Each row in `DRUM_SEQ` corresponds to a step. The four values are for pins 4, 5, 6, and 7.

---

## 3. Code: Stepping Through the Sequence

### CCW (Counterclockwise) Rotation

- The code steps **forward** through the sequence: 0 → 1 → 2 → ... → 7 → 0 ...
- At each step, the pin states are set according to `DRUM_SEQ[step]`.

```cpp
int phase = drumPhase; // for CCW
for (int pin = 0; pin < 4; pin++)
    digitalWrite(DRUM_PINS[pin], DRUM_SEQ[phase][pin]);
```

### CW (Clockwise) Rotation

- The code steps **backward** through the sequence: 7 → 6 → ... → 0 → 7 ...
- This is achieved by reversing the index: `phase = 7 - drumPhase;`

```cpp
int phase = 7 - drumPhase; // for CW
for (int pin = 0; pin < 4; pin++)
    digitalWrite(DRUM_PINS[pin], DRUM_SEQ[phase][pin]);
```

---

## 4. How the Code Alternates CW and CCW

- The variable `drumCCW` is `true` for CCW, `false` for CW.
- In the code, this logic is used:

```cpp
int phase = drumCCW ? drumPhase : (7 - drumPhase);
for (int pin = 0; pin < 4; pin++)
    digitalWrite(DRUM_PINS[pin], DRUM_SEQ[phase][pin]);
```
- The step index (`drumPhase`) is incremented each time, and wraps around after 7.

---

## 5. Visualizing the Steps

At each "step," exactly one or two LEDs light up, simulating a moving spot around the drum.  
By walking through the sequence forwards (CCW) or backwards (CW), you get the effect of the drum spinning in either direction.

---
