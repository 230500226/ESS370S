# Arduino Servo Control with Potentiometer Guide

This guide explains how to build and set up a simple Arduino circuit to control a servo motor using a potentiometer. The circuit reads the position of the potentiometer and moves the servo accordingly.

## Components Needed

- 1x Arduino board (Uno, Nano, Mega, etc.)
- 1x Servo motor (standard 3-wire type)
- 1x Potentiometer (10kΩ recommended)
- Jumper wires
- Breadboard (optional but helpful)
- External power supply for servo (optional, for high-torque or larger servos)

## Circuit Diagram

```
Potentiometer
   +---------+
   |         |
  [ ]       [ ] 
   |         |
  GND      5V (VCC)
   |         |
   +---|<|---+
       |
     Analog pin A0

Servo
   +----+      +-----+
   |    |      |     |
   |    +------+     |
   |           |     |
  GND        5V     Pin 9 (Signal)
```

### Connections

#### Potentiometer
- One outer leg to **5V** on Arduino
- Other outer leg to **GND** on Arduino
- Middle (wiper) leg to **A0** (analog pin 0) on Arduino

#### Servo Motor
- Red wire (VCC) to **5V** on Arduino  
  (or external 5V if your servo draws a lot of current)
- Brown/Black wire (GND) to **GND** on Arduino
- Orange/Yellow wire (Signal) to **Pin 9** on Arduino

> **Tip:** If your servo draws more than 500mA, use an external 5V supply for the servo's VCC and connect all grounds together.

## Arduino Code

```cpp
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  myservo.write(val);                  // sets the servo position according to the scaled value
  delay(15);                           // waits for the servo to get there
}
```

## How It Works

1. **Potentiometer Reading:**  
   The Arduino reads the analog voltage from the potentiometer (range: 0–1023).

2. **Mapping:**  
   The analog reading is mapped to a servo angle (0–180 degrees).

3. **Servo Movement:**  
   The servo motor is set to the mapped position, making its shaft rotate according to the potentiometer knob.

4. **Loop:**  
   The process repeats, allowing real-time control of the servo by turning the potentiometer.

## Step-by-Step Setup

1. **Build the circuit** as described above.
2. **Connect your Arduino** to your computer via USB.
3. **Upload the code** to your Arduino using the Arduino IDE.
4. **Turn the potentiometer knob** and watch the servo move accordingly.

## Troubleshooting

- If the servo doesn't move:
  - Check wiring, especially the signal and power connections.
  - Ensure the potentiometer's wiper is connected to A0.
  - Try powering the servo from an external 5V source if it jitters or doesn't move.

## Applications

- Robotic arms
- RC car steering
- Simple analog controls for projects

---
