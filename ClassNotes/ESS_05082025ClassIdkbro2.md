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

# IR Remote Calculator with BODMAS (Operator Precedence)

This guide demonstrates how to use an IR remote for calculator input on Arduino with operator precedence (BODMAS). The result is displayed on the Serial Monitor.

## Features

- Enter multi-digit numbers and operators in sequence using the IR remote.
- Supports +, -, *, / with BODMAS precedence.
- Result is displayed when pressing '=' on your remote.

## Arduino Code

```cpp
#include <IRremote.h>

const int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;

#define MAX_EXPR_LEN 32   // Max length of the expression string

char expr[MAX_EXPR_LEN + 1] = "";
int exprPos = 0;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println("IR Calculator Ready (BODMAS)!");
  Serial.println("Use remote: [0-9]=numbers, [CH+]=+, [CH-]=-, [VOL+]=*, [VOL-]=/, [EQ]==, [C/CE]=clear");
}

bool isOperator(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/';
}

int precedence(char op) {
  if (op == '+' || op == '-') return 1;
  if (op == '*' || op == '/') return 2;
  return 0;
}

// Shunting Yard: Converts expr string to rpn array, returns length of rpn
int toRPN(const char* expr, char* output, int maxlen) {
  char stack[MAX_EXPR_LEN];
  int stackPos = 0, outPos = 0;
  for (int i = 0; expr[i] && outPos < maxlen - 1; ) {
    if (expr[i] >= '0' && expr[i] <= '9') {
      while (expr[i] >= '0' && expr[i] <= '9' && outPos < maxlen - 1)
        output[outPos++] = expr[i++];
      output[outPos++] = ' '; // delimiter
    } else if (isOperator(expr[i])) {
      while (stackPos > 0 && isOperator(stack[stackPos - 1]) &&
             precedence(stack[stackPos - 1]) >= precedence(expr[i]))
        output[outPos++] = stack[--stackPos], output[outPos++] = ' ';
      stack[stackPos++] = expr[i++];
    } else {
      i++; // skip unknown
    }
  }
  while (stackPos > 0 && outPos < maxlen - 1) {
    output[outPos++] = stack[--stackPos];
    output[outPos++] = ' ';
  }
  output[outPos] = '\0';
  return outPos;
}

// Evaluate RPN expression
float evalRPN(const char* rpn) {
  float stack[MAX_EXPR_LEN];
  int stackPos = 0;
  for (int i = 0; rpn[i]; ) {
    if (rpn[i] >= '0' && rpn[i] <= '9') {
      float num = 0;
      while (rpn[i] >= '0' && rpn[i] <= '9')
        num = num * 10 + (rpn[i++] - '0');
      stack[stackPos++] = num;
      if (rpn[i] == ' ') i++;
    } else if (isOperator(rpn[i])) {
      if (stackPos < 2) return 0; // error
      float b = stack[--stackPos], a = stack[--stackPos], res = 0;
      switch (rpn[i]) {
        case '+': res = a + b; break;
        case '-': res = a - b; break;
        case '*': res = a * b; break;
        case '/': res = b != 0 ? a / b : 0; break;
      }
      stack[stackPos++] = res;
      i++;
      if (rpn[i] == ' ') i++;
    } else {
      i++;
    }
  }
  if (stackPos == 1) return stack[0];
  return 0;
}

void clearExpr() {
  expr[0] = '\0';
  exprPos = 0;
}

void printExpr() {
  Serial.print("Expr: ");
  Serial.println(expr);
}

void loop() {
  if (irrecv.decode(&results)) {
    unsigned long value = results.value;
    bool updated = false;

    switch (value) {
      case 0xFF6897: if(exprPos < MAX_EXPR_LEN) expr[exprPos++] = '0', expr[exprPos] = '\0', Serial.print("0"), updated = true; break;
      case 0xFF30CF: if(exprPos < MAX_EXPR_LEN) expr[exprPos++] = '1', expr[exprPos] = '\0', Serial.print("1"), updated = true; break;
      case 0xFF18E7: if(exprPos < MAX_EXPR_LEN) expr[exprPos++] = '2', expr[exprPos] = '\0', Serial.print("2"), updated = true; break;
      case 0xFF7A85: if(exprPos < MAX_EXPR_LEN) expr[exprPos++] = '3', expr[exprPos] = '\0', Serial.print("3"), updated = true; break;
      case 0xFF10EF: if(exprPos < MAX_EXPR_LEN) expr[exprPos++] = '4', expr[exprPos] = '\0', Serial.print("4"), updated = true; break;
      case 0xFF38C7: if(exprPos < MAX_EXPR_LEN) expr[exprPos++] = '5', expr[exprPos] = '\0', Serial.print("5"), updated = true; break;
      case 0xFF5AA5: if(exprPos < MAX_EXPR_LEN) expr[exprPos++] = '6', expr[exprPos] = '\0', Serial.print("6"), updated = true; break;
      case 0xFF42BD: if(exprPos < MAX_EXPR_LEN) expr[exprPos++] = '7', expr[exprPos] = '\0', Serial.print("7"), updated = true; break;
      case 0xFF4AB5: if(exprPos < MAX_EXPR_LEN) expr[exprPos++] = '8', expr[exprPos] = '\0', Serial.print("8"), updated = true; break;
      case 0xFF52AD: if(exprPos < MAX_EXPR_LEN) expr[exprPos++] = '9', expr[exprPos] = '\0', Serial.print("9"), updated = true; break;
      // Operators
      case 0xFF9867: if(exprPos < MAX_EXPR_LEN) expr[exprPos++] = '+', expr[exprPos] = '\0', Serial.print(" + "), updated = true; break; // CH+
      case 0xFFB04F: if(exprPos < MAX_EXPR_LEN) expr[exprPos++] = '-', expr[exprPos] = '\0', Serial.print(" - "), updated = true; break; // CH-
      case 0xFF629D: if(exprPos < MAX_EXPR_LEN) expr[exprPos++] = '*', expr[exprPos] = '\0', Serial.print(" * "), updated = true; break; // VOL+
      case 0xFFA857: if(exprPos < MAX_EXPR_LEN) expr[exprPos++] = '/', expr[exprPos] = '\0', Serial.print(" / "), updated = true; break; // VOL-
      // Equals
      case 0xFF22DD: { // EQ
        Serial.print(" = ");
        char rpn[MAX_EXPR_LEN * 2] = "";
        toRPN(expr, rpn, sizeof(rpn));
        float result = evalRPN(rpn);
        Serial.println(result);
        clearExpr();
        updated = false;
        break;
      }
      // Clear
      case 0xFFE21D:
        Serial.println("\nCleared");
        clearExpr();
        updated = false;
        break;
      default:
        // Uncomment to debug unknown codes:
        // Serial.print("Unknown: "); Serial.println(value, HEX);
        break;
    }

    if (updated) printExpr();
    irrecv.resume();
  }
}
```

## Usage

- Enter a sequence such as `2`, `+`, `3`, `*`, `4`, then `=`.  
  Output will be `2 + 3 * 4 = 14` (since multiplication has higher precedence).
- To clear, press the `C/CE` key.
- To map different remote codes, print out the unknown codes and update the switch statement.

## Notes

- Parentheses are not supported due to remote input and memory limitations.
- If you need to support floats/decimals, further parsing logic will be required.
- You must map your own IR remote's buttons if the codes differ. Use the Serial Monitor to print unknown codes and update the switch statement accordingly.
- This is a simple two-operand calculator (no operator precedence or parentheses).

---
