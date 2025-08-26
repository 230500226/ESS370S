# Arduino Fridge Simulation (Tinkercad)

Simulate a basic refrigerator controller in Tinkercad using:
- A potentiometer to set the desired internal temperature (setpoint).
- An LED to represent the compressor (on = cooling).
- Serial Monitor to display live status (temperature, setpoint, compressor state).
- A fully simulated thermistor (no physical sensor needed) using a mathematical temperature model inside the sketch.

This guide walks you through:
1. Concept
2. Components
3. Circuit wiring in Tinkercad
4. Temperature & compressor simulation model
5. Step-by-step build
6. Full Arduino code
7. How to run, test, and extend

---

## 1. Concept Overview

We want to mimic a refrigerator control loop:
- User selects desired temperature (e.g. 0–10 °C) with a potentiometer.
- Internal (simulated) fridge temperature changes over time:
  - Cools down when compressor active.
  - Warms toward ambient when compressor idle.
- A hysteresis band prevents rapid toggling.
- LED = compressor state.
- Serial Monitor logs values every second.
- A simulated thermistor model converts the simulated temperature into a fake ADC reading, demonstrating how a real thermistor circuit might behave.

No real thermistor component is added (as requested); instead, code fabricates the reading.

---

## 2. Components (All Available in Tinkercad Circuits)

Required:
- 1 × Arduino Uno R3
- 1 × Potentiometer (10 kΩ)
- 1 × LED (any color)
- 1 × Resistor (220 Ω for LED)
- Breadboard (optional but tidy)

Optional (Not used here unless you expand):
- Additional LEDs for status indicators
- Pushbutton for door simulation
- LCD display (16x2) for offline display

---

## 3. Wiring

Potentiometer (for setpoint):
- One outer leg → 5V
- Other outer leg → GND
- Middle (wiper) leg → Arduino A0

Compressor LED:
- Arduino digital pin 9 → 220 Ω resistor → LED anode
- LED cathode → GND

(Or just use pin 13 & built-in LED, but external LED is clearer.)

Grounds: Ensure all grounds are common (pot & LED share Arduino GND).

No thermistor physically wired (it is simulated in software).

---

## 4. Simulation Model Details

### 4.1 Temperature Dynamics
We simulate internal fridge temperature `tempC` influenced by:
- Ambient temperature `ambientC` (e.g. 22 °C)
- Compressor cooling rate
- Natural warming toward ambient when off
- Small random noise for realism

Pseudo-behavior each loop (every 1 second):
- If compressor ON: tempC -= coolingRate (to a floor just above -5 °C)
- If compressor OFF: tempC += (ambientC - tempC) * passiveWarmFactor
- Add small Gaussian-like or uniform noise (optional)

### 4.2 Setpoint & Hysteresis
- Pot reading (0–1023) mapped to 0–10 °C
- Hysteresis width (e.g. 0.6 °C)
  - Turn compressor ON when tempC > setpoint + (hyst/2)
  - Turn compressor OFF when tempC < setpoint - (hyst/2)

### 4.3 Simulating a Thermistor Reading
We pretend we have:
- 10 kΩ NTC thermistor at 25 °C with Beta = 3950
- Voltage divider: R_fixed (10 kΩ) to +5V, thermistor to GND, analog read at the junction

Equation:
R(T) = R0 * exp( B * (1/Tk - 1/T0) )
Where:
- R0 = 10000 Ω at T0 = 298.15 K (25 °C)
- Tk = (tempC + 273.15) K

ADC value:
Vout = 5 * (R / (R + R_fixed))
ADC = round( Vout / 5 * 1023 )

We then (optionally) "recompute" temperature from simulated ADC to show the processing pipeline. This gives an example of how real hardware code would look.

You can disable simulation of thermistor and just use `tempC` directly by toggling a compile-time flag.

---

## 5. Step-by-Step in Tinkercad

1. Open Tinkercad → Circuits → Create New Circuit.
2. Place an Arduino Uno.
3. Place a potentiometer. Insert across breadboard gap (or same row if using mini breadboard).
4. Wire:
   - Pot left leg → 5V
   - Pot right leg → GND
   - Pot middle leg → A0
5. Place LED + resistor:
   - Pin 9 → resistor → LED anode
   - LED cathode → GND
6. (Optional) Label wires for clarity.
7. Open the code editor → Choose Text mode → Replace contents with code below.
8. Start simulation.
9. Open Serial Monitor (baud 115200). Observe log lines.
10. Turn the potentiometer; setpoint changes; LED toggles as temperature crosses hysteresis thresholds.

---

## 6. Full Arduino Code

Features:
- Simulated fridge temperature dynamics
- Simulated thermistor ADC path (with reconstruction to test)
- Pot-based setpoint
- Compressor LED control with hysteresis
- Periodic serial output
- Optional smoothing & noise

Copy everything below into Tinkercad's code editor.

```cpp
/*
  Fridge Simulation with Potentiometer Setpoint and Simulated Thermistor
  Author: (Your Name)
  Description:
    - Pot on A0 sets desired fridge temperature (0–10 °C).
    - Internal temperature model evolves over time.
    - Thermistor reading is simulated (no physical sensor).
    - LED on pin 9 indicates compressor state (ON = cooling).
    - Serial monitor shows status every second.

  Adjust constants in CONFIG section as desired.
*/

//////////////////// CONFIG ////////////////////
const int PIN_SETPOINT_POT   = A0;
const int PIN_COMPRESSOR_LED = 9;

// Setpoint range (°C)
const float SETPOINT_MIN_C = 0.0;
const float SETPOINT_MAX_C = 10.0;

// Ambient temperature (room)
float ambientC = 22.0;

// Starting internal temperature (begin warm)
float tempC = 15.0;

// Hysteresis width (°C)
float hysteresisWidth = 0.6;

// Cooling & warming dynamics per second
float coolingRatePerSec      = 0.25;  // °C per second while compressor on
float passiveWarmFactor      = 0.06;  // Fraction of difference to ambient per second
float minPhysicalTemp        = -4.0;  // Prevent unrealistic cooling
float noiseAmplitude         = 0.05;  // Random variation (°C)

// Simulated thermistor constants
const bool  USE_SIMULATED_THERMISTOR = true;
const float THERM_R0   = 10000.0;   // 10k at 25°C
const float THERM_BETA = 3950.0;
const float THERM_T0K  = 298.15;    // 25°C in Kelvin
const float DIVIDER_R  = 10000.0;   // Fixed resistor in divider (10k)
const float ADC_REF_V  = 5.0;
const int   ADC_MAX    = 1023;

// Loop timing
unsigned long lastUpdateMs = 0;
const unsigned long UPDATE_INTERVAL_MS = 1000;

bool compressorOn = false;

//////////////////// UTILS ////////////////////

float mapPotToSetpoint(int raw) {
  return SETPOINT_MIN_C + ( (SETPOINT_MAX_C - SETPOINT_MIN_C) * ( (float)raw / 1023.0f ) );
}

// Simulated thermistor raw ADC given the simulated internal temperature
int simulateThermistorADC(float temperatureC) {
  float Tk = temperatureC + 273.15;
  // R(T) using Beta equation
  float R = THERM_R0 * exp( THERM_BETA * (1.0/Tk - 1.0/THERM_T0K) );
  float vOut = ADC_REF_V * (R / (R + DIVIDER_R));
  int adc = (int)round( (vOut / ADC_REF_V) * ADC_MAX );
  if (adc < 0) adc = 0;
  if (adc > ADC_MAX) adc = ADC_MAX;
  return adc;
}

// Reconstruct temperature from ADC (like real sensor reading)
float reconstructTempFromADC(int adcValue) {
  // Vout = 5 * (R / (R + R_fixed)) => R = R_fixed * (Vout / (Vref - Vout))
  float vOut = ((float)adcValue / ADC_MAX) * ADC_REF_V;
  if (vOut <= 0.0f) vOut = 0.0001f;
  if (vOut >= ADC_REF_V) vOut = ADC_REF_V - 0.0001f;

  float R = DIVIDER_R * (vOut / (ADC_REF_V - vOut));
  // Invert Beta formula:
  // 1/Tk = 1/T0 + (1/B) * ln(R/R0)
  float invTk = (1.0/THERM_T0K) + (1.0/THERM_BETA) * log(R / THERM_R0);
  float Tk = 1.0 / invTk;
  return Tk - 273.15;
}

float randomNoise(float amplitude) {
  // Simple uniform noise in [-amplitude, +amplitude]
  long r = random(-10000, 10001);
  return ( (float)r / 10000.0f ) * amplitude;
}

//////////////////// SETUP ////////////////////
void setup() {
  pinMode(PIN_COMPRESSOR_LED, OUTPUT);
  digitalWrite(PIN_COMPRESSOR_LED, LOW);
  pinMode(PIN_SETPOINT_POT, INPUT);
  Serial.begin(115200);
  randomSeed(analogRead(0)); // crude seed
  Serial.println(F("Fridge Simulation Started"));
  Serial.println(F("Columns: ms,setPointC,tempC,compOn,adcSim,thermTempReconC"));
}

//////////////////// LOOP ////////////////////
void loop() {
  unsigned long now = millis();
  if (now - lastUpdateMs >= UPDATE_INTERVAL_MS) {
    lastUpdateMs = now;

    // 1. Read setpoint
    int potRaw = analogRead(PIN_SETPOINT_POT);
    float setPointC = mapPotToSetpoint(potRaw);

    // 2. Update compressor state (hysteresis)
    float upper = setPointC + (hysteresisWidth / 2.0);
    float lower = setPointC - (hysteresisWidth / 2.0);

    if (!compressorOn && tempC > upper) {
      compressorOn = true;
    } else if (compressorOn && tempC < lower) {
      compressorOn = false;
    }

    // 3. Update temperature model
    if (compressorOn) {
      tempC -= coolingRatePerSec;
      if (tempC < minPhysicalTemp) tempC = minPhysicalTemp;
    } else {
      // Warm toward ambient
      tempC += (ambientC - tempC) * passiveWarmFactor;
    }

    // 4. Add noise
    tempC += randomNoise(noiseAmplitude);

    // 5. Simulated thermistor path
    int simulatedADC = -1;
    float reconstructedC = NAN;
    if (USE_SIMULATED_THERMISTOR) {
      simulatedADC = simulateThermistorADC(tempC);
      reconstructedC = reconstructTempFromADC(simulatedADC);
    }

    // 6. Drive compressor LED
    digitalWrite(PIN_COMPRESSOR_LED, compressorOn ? HIGH : LOW);

    // 7. Serial output (CSV-style)
    Serial.print(now);
    Serial.print(',');
    Serial.print(setPointC, 2);
    Serial.print(',');
    Serial.print(tempC, 2);
    Serial.print(',');
    Serial.print(compressorOn ? 1 : 0);
    Serial.print(',');
    if (USE_SIMULATED_THERMISTOR) {
      Serial.print(simulatedADC);
      Serial.print(',');
      Serial.print(reconstructedC, 2);
    } else {
      Serial.print("NA,NA");
    }
    Serial.println();
  }

  // Non-blocking idle
}
```

---

## 7. Running & Observing

1. Start simulation.
2. Open Serial Monitor (115200 baud).
3. Turn potentiometer:
   - Lower setpoint → LED should stay on longer (more cooling).
   - Higher setpoint → LED off more often.
4. Watch reported:
   - setPointC (desired)
   - tempC (internal actual)
   - compOn (0/1)
   - adcSim (fabricated thermistor ADC)
   - thermTempReconC (temperature calculated back from ADC, should track tempC closely)

---

## 8. Tuning Parameters

Edit constants near top:
- ambientC: Raise/lower room warmth.
- coolingRatePerSec: Increase to cool faster.
- passiveWarmFactor: Increase to warm faster when off.
- hysteresisWidth: Increase to reduce cycling frequency.
- noiseAmplitude: Zero it out for a perfectly stable reading.

---

## 9. Possible Extensions

Ideas to expand:
- Add a button to simulate door open → increases tempC or reduces cooling efficiency.
- Add an LCD (I2C or 16x2) to display status without Serial.
- Log data to an SD card (in physical builds).
- Add EEPROM persistence for last setpoint.
- Implement PID instead of simple hysteresis.
- Add defrost cycle logic (periodic compressor off + heater LED).
- Use tone() to beep if temperature exceeds safety threshold.

---

## 10. Troubleshooting

Issue: LED never turns on
- Check wiring to correct pin.
- Ensure setpoint is not above current temperature.
- Lower setpoint with the potentiometer.

Issue: Temperature not changing
- Confirm simulation loop runs (Serial updates each second).
- Verify coolingRatePerSec and passiveWarmFactor not set to 0.

Issue: Reconstructed thermistor temperature drifts slightly
- Minor differences from floating-point rounding & noise are expected.

---

# xor# Arduino Binary Adder (Serial Monitor)

This guide shows how to use an Arduino (e.g., Uno) as a simple binary number adder via the Serial Monitor.  
You will type two binary numbers (like `1011` and `111`) and the Arduino will:
- Validate them
- Add them
- Display:
  - Decimal values of each
  - Binary sum
  - Decimal sum
  - Bit length
  - Overflow notice (if exceeding configured bit width, optional mode)

No extra hardware required—only a USB cable and the Serial Monitor.

---

## 1. Features

- Accepts binary inputs up to a configurable maximum bit length (default 16).
- Ignores leading/trailing whitespace.
- Rejects invalid characters.
- Supports optional fixed-width mode (e.g., 8-bit) with overflow detection and wrap-around.
- Clean restart after each addition cycle.
- Optional command shortcuts:
  - `#max=n` to change maximum accepted bits (2–32)
  - `#mode=free` (default) no forced width, grows to needed bits
  - `#mode=8`, `#mode=16`, etc. to force width and show overflow if needed
  - `#help` to display help
  - `#clear` to clear screen (ANSI attempt)
  - `#restart` to restart prompt

---

## 2. How It Works

1. Arduino initializes Serial at 115200 baud.
2. Prompts: "Enter first binary number:"
3. Reads a line until newline (`\n` or `\r\n`).
4. Validates (only `0` or `1`).
5. Repeats for second number.
6. Converts both to unsigned long (up to 32 bits safely).
7. Adds them.
8. Computes:
   - Raw sum
   - Binary string for sum
   - If in fixed-width mode: applies mask, detects overflow
9. Prints result block.
10. Loops back for another pair.

---

## 3. Upload & Use Steps

1. Open Arduino IDE or Tinkercad Circuits (Code panel → Text).
2. Copy the sketch below.
3. Upload or Start Simulation.
4. Open Serial Monitor:
   - Set baud: 115200
   - Line ending: Newline (recommended)
5. Follow prompts.
6. Optionally change mode, e.g. type `#mode=8` then press Enter before supplying numbers.

---

## 4. Example Session

```
Binary Adder Ready. Type #help for commands.
Enter first binary number:
> 101101
Enter second binary number:
> 1111

---- RESULT ----
A (bin): 101101
B (bin): 001111
A (dec): 45
B (dec): 15
Sum (bin): 111100
Sum (dec): 60
Bits (sum): 6
Mode: free (no forced width)
----------------

Enter first binary number:
```

Fixed-width example after `#mode=8`:

```
Enter first binary number:
> 11110000
Enter second binary number:
> 11110000

---- RESULT ----
A (bin, padded 8): 11110000
B (bin, padded 8): 11110000
A (dec): 240
B (dec): 240
Sum (raw bin): 111100000
Forced width: 8 bits
Sum (truncated bin): 11100000
Sum (truncated dec): 224
OVERFLOW: Raw sum exceeded 8 bits.
----------------
```

---

## 5. Full Arduino Code

```cpp
/*
  Arduino Binary Adder (Serial Monitor)
  ------------------------------------
  Interactively adds two binary numbers typed by the user.

  Commands (enter alone on a line):
    #help        - Show help.
    #max=n       - Set maximum accepted bits for input (2..32).
    #mode=free   - Free mode (no fixed width).
    #mode=8 /16  - Fixed-width mode; overflow detection & truncation.
    #restart     - Restart prompt flow.
    #clear       - Attempt to clear terminal (ANSI escape).
  
  Author: (Your Name)
  License: Public Domain / CC0
*/

const unsigned long SERIAL_BAUD = 115200;

// Configuration
unsigned int maxBitsAccepted = 16;   // Hard cap for entering numbers (modifiable via #max=n)
bool fixedWidthMode = false;         // false = 'free', true = fixed width
unsigned int fixedWidthBits = 8;     // Used only if fixedWidthMode = true

// State machine
enum InputState { WAITING_FIRST, WAITING_SECOND };
InputState state = WAITING_FIRST;

String lineBuffer = "";
String binA = "";
String binB = "";

// Utility: trim & remove spaces inside
String sanitizeInput(const String &in) {
  String s;
  // Remove leading/trailing whitespace manually
  int start = 0;
  while (start < (int)in.length() && isspace(in[start])) start++;
  int end = in.length() - 1;
  while (end >= 0 && isspace(in[end])) end--;
  for (int i = start; i <= end; i++) {
    char c = in[i];
    if (!isspace(c)) s += c; // Remove internal spaces as well
  }
  return s;
}

// Check if string is binary and within length
bool isValidBinary(const String &s) {
  if (s.length() == 0) return false;
  if (s.length() > maxBitsAccepted) return false;
  for (unsigned int i = 0; i < s.length(); i++) {
    char c = s[i];
    if (c != '0' && c != '1') return false;
  }
  return true;
}

// Convert binary string to unsigned long (up to 32 bits safe)
unsigned long binToULong(const String &s) {
  unsigned long value = 0;
  for (unsigned int i = 0; i < s.length(); i++) {
    value <<= 1;
    if (s[i] == '1') value |= 1UL;
  }
  return value;
}

// Convert unsigned long to binary string
String uLongToBin(unsigned long val) {
  if (val == 0) return "0";
  String out;
  while (val > 0) {
    out = ((val & 1UL) ? '1' : '0') + out;
    val >>= 1;
  }
  return out;
}

// Pad binary string to width with leading zeros
String padLeft(const String &s, unsigned int width) {
  if (s.length() >= width) return s;
  String pad;
  for (unsigned int i = 0; i < width - s.length(); i++) pad += '0';
  return pad + s;
}

// Print help
void printHelp() {
  Serial.println(F("Commands:"));
  Serial.println(F("  #help        Show this help"));
  Serial.println(F("  #max=n       Set max input bits (2..32)"));
  Serial.println(F("  #mode=free   Free mode (no fixed width)"));
  Serial.println(F("  #mode=8/16.. Set fixed width & overflow detection"));
  Serial.println(F("  #restart     Restart number entry"));
  Serial.println(F("  #clear       Clear screen (ANSI)"));
}

// Apply command line (returns true if it was command)
bool handleCommand(const String &s) {
  if (s.length() == 0 || s[0] != '#') return false;
  if (s.equalsIgnoreCase("#help")) {
    printHelp();
  } else if (s.startsWith("#max=")) {
    int eq = s.indexOf('=');
    if (eq >= 0) {
      int n = s.substring(eq + 1).toInt();
      if (n >= 2 && n <= 32) {
        maxBitsAccepted = (unsigned int)n;
        Serial.print(F("Set maxBitsAccepted="));
        Serial.println(maxBitsAccepted);
      } else {
        Serial.println(F("Invalid #max value (2..32)."));
      }
    }
  } else if (s.equalsIgnoreCase("#mode=free")) {
    fixedWidthMode = false;
    Serial.println(F("Mode set to free (no forced width)."));
  } else if (s.startsWith("#mode=")) {
    int eq = s.indexOf('=');
    if (eq >= 0) {
      int n = s.substring(eq + 1).toInt();
      if (n >= 2 && n <= 32) {
        fixedWidthMode = true;
        fixedWidthBits = (unsigned int)n;
        Serial.print(F("Fixed-width mode set to "));
        Serial.print(fixedWidthBits);
        Serial.println(F(" bits."));
      } else {
        Serial.println(F("Invalid width (2..32)."));
      }
    }
  } else if (s.equalsIgnoreCase("#restart")) {
    binA = "";
    binB = "";
    state = WAITING_FIRST;
    Serial.println(F("Restarted."));
    Serial.println(F("Enter first binary number:"));
  } else if (s.equalsIgnoreCase("#clear")) {
    // ANSI clear screen (may not work in all terminals)
    Serial.write(27); Serial.print(F("[2J"));
    Serial.write(27); Serial.print(F("[H"));
    Serial.println(F("Screen cleared."));
  } else {
    Serial.println(F("Unknown command. Type #help."));
  }
  return true;
}

void printIntro() {
  Serial.println(F("Binary Adder Ready. Type #help for commands."));
  Serial.print(F("Current max bits: "));
  Serial.println(maxBitsAccepted);
  if (fixedWidthMode) {
    Serial.print(F("Mode: fixed width = "));
    Serial.print(fixedWidthBits);
    Serial.println(F(" bits."));
  } else {
    Serial.println(F("Mode: free (auto width)."));
  }
  Serial.println(F("Enter first binary number:"));
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  // Wait brief moment for serial to open (especially in some environments)
  delay(200);
  printIntro();
}

void processLine(String raw) {
  String s = sanitizeInput(raw);

  // Command?
  if (handleCommand(s)) return;

  if (state == WAITING_FIRST) {
    if (!isValidBinary(s)) {
      Serial.println(F("Invalid binary. Re-enter first binary number:"));
      return;
    }
    binA = s;
    state = WAITING_SECOND;
    Serial.println(F("Enter second binary number:"));
  } else if (state == WAITING_SECOND) {
    if (!isValidBinary(s)) {
      Serial.println(F("Invalid binary. Re-enter second binary number:"));
      return;
    }
    binB = s;
    // Compute result
    unsigned long valA = binToULong(binA);
    unsigned long valB = binToULong(binB);
    unsigned long rawSum = valA + valB;
    String binSum = uLongToBin(rawSum);

    Serial.println();
    Serial.println(F("---- RESULT ----"));

    if (fixedWidthMode) {
      // Pad inputs
      String aPad = padLeft(binA, fixedWidthBits);
      String bPad = padLeft(binB, fixedWidthBits);
      Serial.print(F("A (bin, padded ")); Serial.print(fixedWidthBits); Serial.print(F("): "));
      Serial.println(aPad);
      Serial.print(F("B (bin, padded ")); Serial.print(fixedWidthBits); Serial.print(F("): "));
      Serial.println(bPad);
    } else {
      Serial.print(F("A (bin): "));
      Serial.println(binA);
      Serial.print(F("B (bin): "));
      Serial.println(binB);
    }

    Serial.print(F("A (dec): "));
    Serial.println(valA);
    Serial.print(F("B (dec): "));
    Serial.println(valB);

    if (fixedWidthMode) {
      Serial.print(F("Sum (raw bin): "));
      Serial.println(binSum);

      // Check overflow
      bool overflow = (binSum.length() > fixedWidthBits);

      // Truncate to fixed width (LSBs)
      unsigned long mask = (fixedWidthBits == 32) ? 0xFFFFFFFFUL : ((1UL << fixedWidthBits) - 1UL);
      unsigned long truncated = rawSum & mask;
      String truncatedBin = uLongToBin(truncated);
      truncatedBin = padLeft(truncatedBin, fixedWidthBits);

      Serial.print(F("Forced width: "));
      Serial.print(fixedWidthBits);
      Serial.println(F(" bits"));
      Serial.print(F("Sum (truncated bin): "));
      Serial.println(truncatedBin);
      Serial.print(F("Sum (truncated dec): "));
      Serial.println(truncated);

      if (overflow) {
        Serial.println(F("OVERFLOW: Raw sum exceeded fixed width."));
      } else {
        Serial.println(F("No overflow."));
      }
    } else {
      Serial.print(F("Sum (bin): "));
      Serial.println(binSum);
      Serial.print(F("Sum (dec): "));
      Serial.println(rawSum);
      Serial.print(F("Bits (sum): "));
      Serial.println(binSum.length());
      Serial.println(F("Mode: free (no forced width)"));
    }
    Serial.println(F("----------------"));
    Serial.println();
    // Reset for next session
    binA = "";
    binB = "";
    state = WAITING_FIRST;
    Serial.println(F("Enter first binary number:"));
  }
}

void loop() {
  // Read lines non-blocking
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\r') {
      // Ignore carriage return (common with CRLF)
      continue;
    } else if (c == '\n') {
      processLine(lineBuffer);
      lineBuffer = "";
    } else {
      // Simple input guard: limit line length to avoid huge memory usage
      if (lineBuffer.length() < 80) {
        lineBuffer += c;
      }
    }
  }
}
```

---

## 6. Customization Ideas

- Allow subtraction (`A - B`) with a command toggle.
- Add signed interpretation (two's complement).
- Display intermediate carry bits.
- Implement multi-operand chaining.
- Add hex output alongside binary/decimal.

---

## 7. Troubleshooting

Issue: Nothing appears in Serial Monitor  
- Confirm baud set to 115200.  
- Press reset on Arduino after opening the monitor.

Issue: "Invalid binary" messages  
- Ensure only characters 0 and 1.  
- Length must be ≤ maxBitsAccepted.

Issue: Overflow always triggers in fixed mode  
- Sum may exceed chosen width—choose a larger width (#mode=16) or switch to `#mode=free`.

---

## 8. License

This example is free to use, modify, and distribute for learning purposes.

Enjoy experimenting!
