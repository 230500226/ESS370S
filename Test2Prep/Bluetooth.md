---

# Arduino Bluetooth Module & Switching LEDs via User Input

#### Demos for using a Bluetooth serial module and parsing user commands to control LEDs.

---

## 1. Using a Bluetooth Module for Serial Communication (Section 9.3)

---

### Circuit Link

*(No link provided)*

---

### Example Code

```cpp
#include <SoftwareSerial.h>

SoftwareSerial btSerial(2, 3); // 2 = RX (receive), 3 = TX (transmit)
String blueIn;

void setup() {
  btSerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // Read from Bluetooth and send to Serial Monitor
  if (btSerial.available() > 0) {
    blueIn = btSerial.readString();
    Serial.println(blueIn);
  }

  // Read from Serial Monitor and send to Bluetooth
  if (Serial.available() > 0) {
    blueIn = Serial.readString();
    btSerial.println(blueIn);
  }
}
```

---

### Notes

- Use `SoftwareSerial` to create additional serial ports (pins 2 and 3 in this example).
- Any data sent from the phone (over Bluetooth) can be displayed in the Serial Monitor.
- Any data sent from the Serial Monitor can be sent to the Bluetooth device (e.g., phone).
- Swap the `.readString()` and `.println()` calls as needed to switch the direction of communication.
- Useful for relaying messages between a Bluetooth device and the Arduino IDE on your computer.

---

## 2. Switch On Specific LED with User Input (Section 9.4)

---

### Circuit Link

[https://www.tinkercad.com/things/g4HbM50oHHv-design-94](https://www.tinkercad.com/things/g4HbM50oHHv-design-94)

---

### Example Code

```cpp
int startPos, endPos, posLED;
String userIn, partString, stringPiece;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    userIn = Serial.readStringUntil('\n');
    Serial.println(userIn);
    userIn.trim();
    endPos = userIn.indexOf(");");
    startPos = userIn.indexOf("On(");
    if ((startPos > -1) && (endPos > startPos)) {
      Serial.print("StartPos =");
      Serial.println(startPos);
      startPos = startPos + 3;
      Serial.print("endPos =");
      Serial.println(endPos);
      stringPiece = userIn.substring(startPos, endPos);
      Serial.print("stringPiece = ");
      Serial.println(stringPiece);
      posLED = stringPiece.toInt();
      digitalWrite(posLED, HIGH);
    } else {
      Serial.println("Nothing to switch on");
    }
  }
}
```

---

### Notes

- This code parses input like `On(5);` from the Serial Monitor and switches on the LED at the specified pin.
- Finds the position of `"On("` and `");"` to extract the pin number.
- Converts the substring between `"On("` and `");"` to an integer with `.toInt()`.
- Example: Sending `On(9);` will turn on the LED connected to pin 9.
- You can expand this code to handle more commands or to turn off LEDs.

---
