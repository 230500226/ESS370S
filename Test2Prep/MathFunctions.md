---

# Math Functions for Wave Generation and Sensor Mapping

This page summarizes common Arduino math functions—such as sawtooth, sine, and map—for generating waveforms and working with sensor data. It follows a practical demo/code-based format.

---

## 1. Sawtooth Wave (Section 11.1)

### **Graphs**

_Sawtooth waveform generation using Arduino serial output._

### **Circuit Link**

[https://www.tinkercad.com/things/97jHLx7H8Mn-design-111](https://www.tinkercad.com/things/97jHLx7H8Mn-design-111)

---

### **Example Code: Sawtooth Wave**

```cpp
// C++ code for generating a sawtooth wave using Serial output
int samples;
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(2000000);
}

void loop()
{
  for(samples = 0; samples < 50; samples++){
    Serial.println(samples);
    delay(100);
  }
}
```

- **Serial Output:** Prints increasing numbers (0–49) in a loop, which forms a sawtooth pattern if plotted.
- **Delay:** Controls how fast the numbers output, affecting the slope.

---

### **Notes**
- Adjust `samples < 50` or `delay(100)` to change the period and frequency of the wave.
- Visualize the waveform by plotting Serial Monitor data.

---

## 2. Sine Wave (Section 11.2)

### **Graphs**

_Sine waveform generation with Arduino and Serial output._

### **Circuit Link**

[https://www.tinkercad.com/things/kygIWPffmee-design-112](https://www.tinkercad.com/things/kygIWPffmee-design-112)

---

### **Example Code: Sine Wave**

```cpp
int samples, amplitude_I;
double amplitude_F, angle;

void setup() {
    Serial.begin(2000000);
}

void loop() {
    for(samples = 0; samples < 200; samples++) {
        angle = samples * 2 * PI / 200;
        amplitude_F = 10 * sin(angle);
        Serial.println(amplitude_F);
        delay(200);
    }
}
```

---

### **Key Components**
| Variable      | Purpose                               |
| ------------- | ------------------------------------- |
| `samples`     | Counter for loop iterations           |
| `angle`       | Angle in radians (0 to 2π)            |
| `amplitude_F` | Calculated sine wave value            |
| `amplitude_I` | *Unused in this code*                 |

---

### **How the Sine Wave is Generated**

- **Angle Calculation:**  
  `angle = samples * 2 * PI / N;`  
  (where N is number of points per cycle, e.g., 200)
- **Sine Value:**  
  `amplitude_F = 10 * sin(angle);`  
  (output range: -10 to +10)
- **Output:**  
  `Serial.println(amplitude_F);`  
  (for plotting)
- **Delay:**  
  `delay(200);`  
  (controls output rate)

---

### **Visualization**
- One full sine wave cycle with `N` (e.g., 200) points.
- Remove or reduce delay for higher frequency.

---

## 3. Map Function (Section 11.4)

### **Graphs**

_Use of map() to rescale sensor data, e.g., ultrasonic distance to LED brightness._

### **Circuit Link**

[https://www.tinkercad.com/things/i1K93DNUONZ-114](https://www.tinkercad.com/things/i1K93DNUONZ-114)

---

### **Example Code: Mapping Distance to Brightness**

```cpp
const int trigPin = 6;  // Trigger pin
const int echoPin = 7; // Echo pin

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Send 10μs pulse to trigger
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;
  
  int brightness = map(distance, 0, 200, 255, 0);
  brightness = constrain(brightness, 0, 255);
 
  analogWrite(3, brightness);
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  Serial.print("Brightness: ");
  Serial.println(brightness);
  
  delay(100);
}
```

---

### **Notes on map()**

- The `map()` function in Arduino remaps a value from one input range to another output range.
- Syntax:  
  ```arduino
  map(value, fromLow, fromHigh, toLow, toHigh)
  ```
- Use `constrain()` to limit output within bounds.
- **Example:**  
  ```arduino
  int sensorValue = analogRead(A0);
  int brightness = map(sensorValue, 0, 1023, 0, 255);
  analogWrite(9, brightness);
  ```

- **Returns:**  
  A long integer. For float mapping, use custom code.

---

#### **How map() Works**

- **Linear interpolation:**
  ```arduino
  mappedValue = (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
  ```
- Does NOT constrain out-of-range values—use `constrain()` if needed.

---

## 4. Potentiometer (Theory)

A **potentiometer** (pot) is a variable resistor for manual adjustment of voltage or resistance.

### **Images**
![[Pasted image 20250521021722.png]]
![[Pasted image 20250521021738.png]]

---

### **How It Works**

- **3 Terminals:**  
  - Terminal 1 (V+), Terminal 2 (Wiper), Terminal 3 (GND)
- **Voltage Divider Mode:**  
  - Vout = Vin × (R2 / (R1 + R2))
- **Variable Resistance Mode:**  
  - Two-terminal use; acts as a rheostat.

---

### **Types**
- Rotary, Linear/Slider, Digital, Trimpot

### **Applications**
- Volume, brightness, position sensors, calibration, etc.

---

## 5. Potentiometer Reading in % (Section 11.5)

### **Graphs**

_Convert potentiometer analog input to percentage (0–100%)._

### **Circuit Link**

[https://www.tinkercad.com/things/42vTppMm0aw-115](https://www.tinkercad.com/things/42vTppMm0aw-115)

---

### **Example Code: Potentiometer to %**

```cpp
int potIn;

void setup() {
    Serial.begin(9600);
    pinMode(3, OUTPUT);
    pinMode(A5, INPUT);
}

void loop() {
    potIn = analogRead(A5);
    potIn = map(potIn, 4, 1023, 0, 100);
    Serial.println(potIn);
}
```

- Uses `map()` to convert analog value (4–1023) to percent (0–100).

---

### **Notes**
- The total resistance of the potentiometer is constant.
- Only the ratio between R1 and R2 changes as the knob is turned.

---

## 6. Angle, Cosine, and Light Intensity (Section 11.6)

### **Graphs**

_Use potentiometer to set angle, compute cosine for light intensity, and map to PWM output._

### **Circuit Link**

[https://www.tinkercad.com/things/g2OiEZGkWNB-116](https://www.tinkercad.com/things/g2OiEZGkWNB-116)

---

### **Example Code: Angle to Cosine to PWM**

```cpp
int potIn;
int angle, intensity, inten_LED_1;
double angle_R;

void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(A5, INPUT);
}

void loop() {
  potIn = analogRead(A5);
  angle = map(potIn, 4, 1023, 0, 360);
  angle_R = angle * PI / 180.0;
  intensity = 1000 * cos(angle_R);
  inten_LED_1 = map(intensity, -1000, 1000, 0, 255);
  Serial.println(inten_LED_1);
  analogWrite(3, inten_LED_1);
}
```

---

### **Code Workflow**

1. **Read Potentiometer:**  
   `potIn = analogRead(A5);`
2. **Map to Angle:**  
   `angle = map(potIn, 4, 1023, 0, 360);`
3. **Convert to Radians:**  
   `angle_R = angle * PI / 180.0;`
4. **Cosine Calculation:**  
   `intensity = 1000 * cos(angle_R);`
5. **Map to PWM 0–255:**  
   `inten_LED_1 = map(intensity, -1000, 1000, 0, 255);`
6. **Output to LED:**  
   `analogWrite(3, inten_LED_1);`

---

### **Notes**
- `cos(angle_R)` yields -1 to 1, so multiply by 1000 for whole numbers.
- Map from -1000 to 1000 (cosine) to 0–255 (LED PWM).
- LED at max brightness at 0°, off at 180°, half at 90°/270°.

---
