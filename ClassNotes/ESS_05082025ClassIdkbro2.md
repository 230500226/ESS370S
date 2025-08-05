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



calculator
# IR Remote Calculator with Arduino

This guide demonstrates how to use an IR remote as a calculator input device for an Arduino. The IR remote buttons serve as numeric and operation keys, and the calculation result is displayed via the Arduino Serial Monitor.

## Hardware Required

- Arduino board (Uno, Nano, etc.)
- IR receiver module (e.g., TSOP38238)
- IR remote control (common TV/DVD remote)
- Jumper wires
- Breadboard

## Circuit Diagram

- Connect the IR receiver OUT pin to Arduino digital pin 2 (can be changed in code).
- Connect VCC and GND of the IR receiver to 5V and GND on Arduino.

```
IR Receiver    Arduino
  OUT    --->   D2
  VCC    --->   5V
  GND    --->   GND
```

## Libraries Needed

- [IRremote by shirriff](https://github.com/z3t0/Arduino-IRremote)

Install via Arduino Library Manager:  
**Sketch > Include Library > Manage Libraries... > Search "IRremote" > Install**

## Arduino Code

```cpp
#include <IRremote.h>

const int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;

String input = "";
char lastOp = 0;
float operand1 = 0;
bool newInput = true;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println("IR Calculator Ready!");
  Serial.println("Use remote: [0-9]=numbers, [CH+]=+, [CH-]=-, [VOL+]=*, [VOL-]=/, [EQ]==");
}

void loop() {
  if (irrecv.decode(&results)) {
    unsigned long value = results.value;
    // Map your remote's codes here. Example uses NEC remote codes.

    switch (value) {
      case 0xFF6897: input += "0"; Serial.print("0"); break; // 0
      case 0xFF30CF: input += "1"; Serial.print("1"); break; // 1
      case 0xFF18E7: input += "2"; Serial.print("2"); break; // 2
      case 0xFF7A85: input += "3"; Serial.print("3"); break; // 3
      case 0xFF10EF: input += "4"; Serial.print("4"); break; // 4
      case 0xFF38C7: input += "5"; Serial.print("5"); break; // 5
      case 0xFF5AA5: input += "6"; Serial.print("6"); break; // 6
      case 0xFF42BD: input += "7"; Serial.print("7"); break; // 7
      case 0xFF4AB5: input += "8"; Serial.print("8"); break; // 8
      case 0xFF52AD: input += "9"; Serial.print("9"); break; // 9
      case 0xFF9867: // CH+ = '+'
        operand1 = input.toFloat();
        lastOp = '+';
        input = "";
        Serial.print(" + ");
        break;
      case 0xFFB04F: // CH- = '-'
        operand1 = input.toFloat();
        lastOp = '-';
        input = "";
        Serial.print(" - ");
        break;
      case 0xFF629D: // VOL+ = '*'
        operand1 = input.toFloat();
        lastOp = '*';
        input = "";
        Serial.print(" * ");
        break;
      case 0xFFA857: // VOL- = '/'
        operand1 = input.toFloat();
        lastOp = '/';
        input = "";
        Serial.print(" / ");
        break;
      case 0xFF22DD: // EQ = '='
        float operand2 = input.toFloat();
        float result;
        switch (lastOp) {
          case '+': result = operand1 + operand2; break;
          case '-': result = operand1 - operand2; break;
          case '*': result = operand1 * operand2; break;
          case '/': result = (operand2 != 0) ? (operand1 / operand2) : 0; break;
          default: result = operand2; break;
        }
        Serial.print(" = ");
        Serial.println(result);
        input = "";
        lastOp = 0;
        operand1 = 0;
        break;
      case 0xFFE21D: // C/CE/Clear
        Serial.println("\nCleared");
        input = "";
        lastOp = 0;
        operand1 = 0;
        break;
      default:
        // Unknown code; ignore or print for debugging
        // Serial.print("Unknown: "); Serial.println(value, HEX);
        break;
    }
    irrecv.resume();
  }
}
```

## Usage

1. Open Serial Monitor at 9600 baud.
2. Use the IR remote to enter numbers and operations:
    - Number keys: 0-9
    - `CH+` for `+`, `CH-` for `-`
    - `VOL+` for `*`, `VOL-` for `/`
    - `EQ` for `=`
    - `C/CE` or similar key for clearing input
3. Results will be printed in the Serial Monitor.

## Notes

- You must map your own IR remote's buttons if the codes differ. Use the Serial Monitor to print unknown codes and update the switch statement accordingly.
- This is a simple two-operand calculator (no operator precedence or parentheses).

---
