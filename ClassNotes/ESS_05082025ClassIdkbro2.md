---

# Arduino IR Remote Control Setup Guide

This guide walks you through setting up and understanding the sample Arduino code using the **IRremote** library to control a digital output (e.g., LED) with an IR remote.

---

## Components Required

- Arduino board (Uno, Nano, etc.)
- IR Receiver module (e.g., VS1838B)
- IR Remote control
- Jumper wires
- Breadboard (optional)
- LED (optional, for visual feedback)
- 220Ω resistor (if using LED)

---

## Circuit Diagram

| Arduino Pin | Connects To          |
|-------------|----------------------|
| 7           | IR Receiver Signal   |
| 5V          | IR Receiver VCC      |
| GND         | IR Receiver GND      |
| 6           | Positive leg of LED  |
| GND         | Negative leg of LED (via 220Ω resistor) |

---

## Step 1: Install the IRremote Library

1. Open the Arduino IDE.
2. Go to **Sketch > Include Library > Manage Libraries...**
3. Search for `IRremote` by Armin Joachimsmeyer.
4. Click **Install**.

---

## Step 2: Connect the Hardware

- Connect the IR Receiver’s **signal pin** to Arduino **Pin 7**.
- Connect the **VCC** and **GND** pins of the IR receiver to **5V** and **GND** on Arduino.
- (Optional) Connect an LED (with resistor) to **Pin 6** for ON/OFF feedback.

---

## Step 3: Upload the Code

Paste the following code into your Arduino IDE:

```cpp
#include <IRremote.hpp>

#define IR_RECEIVE_PIN 7
#define ON_PIN 6

bool on_state;

void setup() {
  Serial.begin(115200); // serial communication
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start receiver

  // setup key states
  on_state = false;
  pinMode(ON_PIN, OUTPUT);
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.println("----- ------ ----- -----");
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);// Print "old" raw data
    Serial.println("----- ------ ----- -----");
    IrReceiver.printIRResultShort(&Serial); // print complete received data in one line
    Serial.println("----- ------ ----- -----");
    IrReceiver.printIRSendUsage(&Serial); // Print statement required to send this data
    Serial.println("----- ------ ----- -----");

    IrReceiver.resume(); // Enable receiving of next value

    // Check the received command and perform actions
    uint16_t cmd = IrReceiver.decodedIRData.command;
    switch (cmd) {
      case 0x0:
        digitalWrite(ON_PIN, !on_state);
        on_state = !on_state;
        break;
      case 0x1:
        // '+' pressed
        break;
      case 0x8:
        // '-' pressed
        break;
      case 0x9:
        // '0' pressed
        break;
      default:
        Serial.print("Command Given: ");
        Serial.println(cmd);
    }
  }
}
```

---

## Step 4: Test the System

1. Open the **Serial Monitor** (set baud rate to **115200**).
2. Point your IR remote at the receiver and press a button.
3. Observe the serial output for raw data and recognized commands.
4. When the button corresponding to command `0x0` is pressed, the output pin (Pin 6) toggles (e.g., LED turns ON/OFF).

---

## Code Explanation

- **IRremote.hpp**: This library handles IR signal decoding.
- **IR_RECEIVE_PIN**: Pin 7 receives IR signals.
- **ON_PIN**: Pin 6 controls an output (like an LED).
- **on_state**: Tracks the ON/OFF state.
- **setup()**: Initializes serial communication, IR receiver, and output pin.
- **loop()**:  
  - Checks for IR signal with `IrReceiver.decode()`.
  - Prints detailed IR signal data to Serial Monitor.
  - Extracts the command from the IR data:
    - If command is `0x0`, toggles ON_PIN and the `on_state` variable.
    - Other command cases (`0x1`, `0x8`, `0x9`) are placeholders for future actions.
    - If an unknown command is received, it's printed to the serial monitor.

---

## Customizing for Your Remote

- Use the serial monitor to view the command codes for each button.
- Update the `switch(cmd)` cases to assign actions to additional buttons as needed.

---

## Troubleshooting

- Ensure the IR receiver is correctly wired.
- Check that the IRremote library is the latest version.
- Make sure the serial monitor baud rate matches the code.
- If nothing appears in Serial Monitor, check the IR receiver orientation and connections.

---
