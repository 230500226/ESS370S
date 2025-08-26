---
# Arduino IR Remote Character Input with LCD & LED Control

This step-by-step guide will show you how to use an IR remote to select and input characters, display them on an LCD screen, and switch on an LED (on pin 12) when the sequence "ON" is entered.

---

## Components Required

- Arduino board (Uno, Nano, etc.)
- IR Receiver module (e.g., VS1838B)
- IR Remote control
- 16x2 LCD screen (with I2C adapter recommended)
- Jumper wires
- Breadboard (optional)
- LED (for ON indication)
- 220Ω resistor (for LED)

---

## Circuit Diagram

| Arduino Pin | Connects To           |
|-------------|-----------------------|
| 7           | IR Receiver Signal    |
| 5V          | IR Receiver VCC       |
| GND         | IR Receiver GND       |
| A4 (SDA)    | LCD SDA (I2C only)    |
| A5 (SCL)    | LCD SCL (I2C only)    |
| 12          | Anode of LED (+)      |
| GND         | Cathode of LED (via 220Ω resistor) |

---

## Step 1: Install Required Libraries

1. Open Arduino IDE.
2. Go to **Sketch > Include Library > Manage Libraries...**
3. Search and install:
    - `IRremote` by Armin Joachimsmeyer
    - `LiquidCrystal_I2C` (for I2C LCDs)

---

## Step 2: Connect the Hardware

- IR Receiver: signal to Pin 7, VCC to 5V, GND to GND.
- LCD: connect SDA to A4, SCL to A5, VCC and GND as needed.
- LED: anode to Pin 12 (with resistor), cathode to GND.

---

## Step 3: Upload the Code

Paste the following code into your Arduino IDE:

```cpp
#include <IRremote.hpp>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define IR_RECEIVE_PIN 7
#define LED_PIN 12

// Adjust to your LCD's I2C address (0x27 or 0x3F are common)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char charSet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";
const int numChars = sizeof(charSet) - 1;
int charIndex = 0;

String inputBuffer = "";

void setup() {
  Serial.begin(115200);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Select Char: ");
  lcd.setCursor(0,1);
  lcd.print(charSet[charIndex]);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  if (IrReceiver.decode()) {
    uint16_t cmd = IrReceiver.decodedIRData.command;

    // Map remote commands (You may need to adjust these for your remote!)
    switch (cmd) {
      case 0x18: // Up/Next char (example code, change for your remote)
        charIndex = (charIndex + 1) % numChars;
        break;
      case 0x52: // Down/Prev char
        charIndex = (charIndex - 1 + numChars) % numChars;
        break;
      case 0x08: // OK/Select (adds char)
        inputBuffer += charSet[charIndex];
        break;
      case 0x1C: // CLR (Clear input)
        inputBuffer = "";
        break;
      case 0x5A: // DEL (Delete last char)
        if (inputBuffer.length() > 0) inputBuffer.remove(inputBuffer.length() - 1);
        break;
      default:
        break;
    }

    // Display selected char and input buffer
    lcd.setCursor(0,0);
    lcd.print("Select Char:      ");
    lcd.setCursor(0,1);
    lcd.print("                "); // clear line
    lcd.setCursor(0,1);
    lcd.print(inputBuffer);
    lcd.setCursor(15,0);
    lcd.print(charSet[charIndex]);

    // If "ON" is found anywhere in inputBuffer, turn LED on
    if (inputBuffer.indexOf("ON") != -1) {
      digitalWrite(LED_PIN, HIGH);
      lcd.setCursor(12,1);
      lcd.print("LED!");
    } else {
      digitalWrite(LED_PIN, LOW);
      lcd.setCursor(12,1);
      lcd.print("    ");
    }

    IrReceiver.resume();
  }
}
```

---

### **Remote Button Mapping**

**You will need to adjust the codes (`cmd` values)** for your remote.  
- Use the Serial Monitor at 115200 baud to see which `command` value each button sends.
- Typical assignments:
    - `Up`/`Next`: Move to next character
    - `Down`/`Prev`: Move to previous character
    - `OK`/`Select`: Add selected character to input
    - `CLR`: Clear input buffer
    - `DEL`: Delete last character

Update the `switch(cmd)` in the code as needed.

---

## Step 4: Using the System

1. Use your IR remote’s navigation keys to scroll through the character set (A-Z, 0-9, space).
2. Press the select/OK key to add the highlighted character to your input.
3. The string being built displays on the LCD.
4. If the input string contains "ON" (anywhere), the LED on pin 12 will light up and "LED!" will show on the LCD.
5. Use CLR to clear all input, and DEL to remove the last character.
6. Adjust button mappings if your remote sends different codes.

---

## Troubleshooting

- If nothing is displayed, check your LCD I2C address and wiring.
- Use the Serial Monitor to see IR command codes for your remote and adjust in the code.
- Make sure the IR receiver is correctly oriented and wired.
- The LED will only light when "ON" (uppercase) is present in the input string; adjust logic for lowercase if needed.

---

## Customization

- Expand `charSet` to include more/special characters.
- Modify the logic to detect other trigger words.
- Add "Enter" button handling to submit the input string.

---
