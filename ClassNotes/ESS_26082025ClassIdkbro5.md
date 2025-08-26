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
