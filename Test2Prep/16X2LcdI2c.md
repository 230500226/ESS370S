---

# 16x2 LCD with I2C Interface

#### Demo for controlling a 16x2 LCD via I2C - TODO

---

## 1. Pinout (I2C Module)

- **GND:** Ground
- **VCC:** +5V supply
- **SDA:** Data line (connects to microcontroller SDA, e.g., A4 on Arduino Uno)
- **SCL:** Clock line (connects to microcontroller SCL, e.g., A5 on Arduino Uno)

---

## 2. How it Works

The 16x2 LCD with I2C module uses the I2C protocol to send data and commands, significantly reducing the number of pins needed (from 6+ to just 2 data lines). The I2C backpack typically uses a PCF8574 chip for communication.

**Key Concepts:**
- The display address is usually 0x27 or 0x3F (can be scanned).
- Data and commands are sent serially.
- The LiquidCrystal_I2C library simplifies usage.

---

## 3. I2C Address

Before connecting, determine the LCD’s I2C address:
- Most common addresses: 0x27 or 0x3F.
- You can use an I2C scanner sketch to find the address.

---

## 4. Wiring Diagram

```plaintext
   Microcontroller    I2C LCD Module    LCD
   --------------    -------------    ----------
   GND           --> GND           --> GND
   +5V           --> VCC           --> VCC
   SDA (A4)      --> SDA
   SCL (A5)      --> SCL
```

- For Arduino Uno: SDA = A4, SCL = A5.
- For Arduino Mega: SDA = 20, SCL = 21.

---

## 5. Example Code Snippet (Arduino/C++)

**Install the library:**  
In Arduino IDE: Tools > Manage Libraries > Search “LiquidCrystal_I2C” > Install

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // (address, columns, rows)

void setup() {
  lcd.init();                      // Initialize LCD
  lcd.backlight();                 // Turn on backlight
  lcd.setCursor(0, 0);             // Set cursor to column 0, row 0
  lcd.print("Hello, World!");      // Print text
  lcd.setCursor(0, 1);             // 2nd row
  lcd.print("I2C LCD Demo");
}

void loop() {
  // Optional: animate text, clear, change content, etc.
}
```

---

## 6. Common Functions

| Function                | Purpose                                |
|-------------------------|----------------------------------------|
| lcd.init()              | Initialize LCD                         |
| lcd.backlight()         | Turn on backlight                      |
| lcd.noBacklight()       | Turn off backlight                     |
| lcd.setCursor(col,row)  | Set cursor position (col, row)         |
| lcd.print("text")       | Print text at cursor                   |
| lcd.clear()             | Clear display                          |
| lcd.scrollDisplayLeft() | Scroll text left                       |
| lcd.scrollDisplayRight()| Scroll text right                      |

---

## 7. Troubleshooting

- If nothing displays, check the I2C address and wiring.
- Adjust the potentiometer on the I2C board if text is not visible (contrast).
- Use an I2C scanner sketch if unsure of address.

---

## 8. References

- [LiquidCrystal_I2C GitHub](https://github.com/johnrickman/LiquidCrystal_I2C)
- [I2C Scanner Sketch (Arduino Playground)](https://playground.arduino.cc/Main/I2cScanner/)
- [16x2 LCD I2C Tutorial](https://lastminuteengineers.com/i2c-lcd-arduino-tutorial/)

---
