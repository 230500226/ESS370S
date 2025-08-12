# Drawing Individual (Custom) Pixels on a 16x2 I2C LCD (HD44780) with Arduino

Yes, you can simulate per‑pixel graphics on a 16x2 character LCD by dynamically redefining the LCD’s 8 custom characters (CGRAM). This guide walks you step by step through:

1. Understanding the pixel grid
2. Mapping (x,y) coordinates
3. Creating custom character bitmaps
4. Writing a helper `setPixel(x,y)` function
5. Managing the 8‑glyph CGRAM limit
6. Example animations and tips

---

## 1. What You Can (and Cannot) Do

- The LCD natively displays 5x8 (or 5x10) dot character cells; most common 16x2 modules use 5x8.
- Total drawable logical pixel area (for 16x2):
  - Width: 16 characters * 5 dots = 80 pixels
  - Height: 2 rows * 8 dots = 16 pixels (often the bottom row (row index 7) is used for underline cursor; still usable for graphics)
- Only 8 custom glyphs (indexes 0–7) can exist at any moment.
- If you need more than 8 distinct patterns visible simultaneously, you must:
  - Reuse patterns
  - Or dynamically swap CGRAM definitions (which can cause visible flicker if overused)

---

## 2. Hardware Setup (Typical I2C Backpack)

| LCD Pin | Backpack (PCF8574) | Arduino (Uno) |
|---------|--------------------|---------------|
| VCC     | —                  | 5V            |
| GND     | —                  | GND           |
| SDA     | SDA                | A4 (SDA)      |
| SCL     | SCL                | A5 (SCL)      |
| Pot / Contrast | On backpack | —             |
| Backlight | On backpack      | —             |

Most I2C LCD backpacks default to addresses like `0x27` or `0x3F`. You can scan with an I2C scanner sketch if unsure.

---

## 3. Library Options

You can use one of:
- `LiquidCrystal_I2C` (commonly available)
- `hd44780` library (more advanced, auto-detects I2C expander)
- `NewLiquidCrystal` (legacy but feature rich)

Examples below assume `LiquidCrystal_I2C`.

Install through Library Manager: Sketch > Include Library > Manage Libraries… search for "LiquidCrystal I2C".

---

## 4. Character Cell and Pixel Coordinate System

Define a global pixel coordinate system:

- X: 0 … 79 (left → right)
- Y: 0 … 15 (top → bottom)

Mapping a pixel (x,y) to a character cell:

```
charCol = x / 5           (integer division, 0–15)
charRow = y / 8           (0–1)
withinCol = x % 5         (0–4) -> bit position horizontally
withinRow = y % 8         (0–7) -> row inside glyph
```

Each glyph bitmap is 8 bytes (rows). Only the lower 5 bits per byte are used (bits 0–4). Bits 5–7 ignored.

---

## 5. Creating a Custom Character

Example: A diagonal line inside one cell.

Row bits (bit 4 is left or right? Conventionally bit 4 = leftmost when using typical libs? Actually in HD44780 the low bits correspond to rightmost or leftmost depending on library’s createChar implementation. For LiquidCrystal-style libs: bit 0 = least significant bit = usually rightmost column. So we’ll adopt column 0 = leftmost by reversing bits manually or just treat bit 4 as leftmost with a helper.)

We’ll build rows as 5-bit patterns:

```
Row0: 10000  (binary) = 0b10000 = 16
Row1: 01000  = 8
Row2: 00100  = 4
Row3: 00010  = 2
Row4: 00001  = 1
Row5: 00000  = 0
Row6: 00000  = 0
Row7: 00000  = 0
```

Code:

```cpp
byte diag[8] = {
  0b10000,
  0b01000,
  0b00100,
  0b00010,
  0b00001,
  0b00000,
  0b00000,
  0b00000
};

lcd.createChar(0, diag); // store in slot 0
lcd.setCursor(0,0);
lcd.write(byte(0));
```

---

## 6. Strategy for Per-Pixel Drawing

You need to maintain a shadow buffer representing 16*2 character cells, each cell storing 8 bytes (its glyph). Then:

1. When `setPixel(x,y)` is called:
   - Compute `(charCol,charRow)`
   - Locate the glyph buffer for that cell
   - Set the bit in the correct row
   - Assign (or reassign) that glyph to one of the 8 CGRAM slots
2. You must track which cell currently uses which CGRAM index (0–7).
3. If > 8 distinct modified cells needed, either:
   - Limit drawing area
   - Or implement a replacement scheme (could cause overwriting another cell’s appearance)

Simplification for a small demo: restrict drawing to the first 8 character cells (e.g., first 8 columns on row 0) so each cell gets its own fixed CGRAM slot (0–7).

---

## 7. Minimal Working Example: Light Single Pixel

This example:
- Maps an 8-cell (8x1 character) canvas (width = 8*5 = 40 pixels, height = 8 pixels) to 8 CGRAM slots
- Provides `setPixel(x,y)` for x in 0–39, y in 0–7

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Each of 8 cells has an 8x5 bitmap (only 8 rows, 5 columns)
// buffer[cellIndex][row] -> 5 LSBs used
byte glyphBuffer[8][8];
bool dirty[8];

void uploadGlyph(uint8_t cellIndex) {
  lcd.createChar(cellIndex, glyphBuffer[cellIndex]);
  dirty[cellIndex] = false;
}

void initGlyphs() {
  for (int c=0; c<8; c++) {
    for (int r=0; r<8; r++) glyphBuffer[c][r] = 0;
    dirty[c] = true;
  }
}

void drawCanvas() {
  lcd.setCursor(0,0);
  for (int c=0; c<8; c++) {
    lcd.write((uint8_t)c);
  }
}

void setPixel(int x, int y) {
  if (x < 0 || x >= 40 || y < 0 || y >= 8) return;

  int cell = x / 5;
  int bitPos = x % 5;       // 0..4
  int row = y;              // 0..7

  // We choose bit 4 as leftmost => convert bitPos to bit index = 4 - bitPos
  int bitIndex = 4 - bitPos;

  byte mask = (1 << bitIndex);
  if (!(glyphBuffer[cell][row] & mask)) {
    glyphBuffer[cell][row] |= mask;
    dirty[cell] = true;
  }
}

void refreshDirty() {
  for (int c=0; c<8; c++) {
    if (dirty[c]) uploadGlyph(c);
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
  initGlyphs();
  drawCanvas();

  // Example: set a diagonal across 40x8 region limited to first row of characters
  for (int x=0; x<40; x++) {
    int y = x / 5; // slow slope
    if (y < 8) setPixel(x, y);
  }
  refreshDirty();
}

void loop() {
  // Nothing
}
```

---

## 8. Full Generalized 80x16 Concept (Explanation)

To cover the entire 16x2 (80x16) pixel area you would need 32 character cells (16 per row * 2 rows). Only 8 CGRAM slots exist. So you cannot simultaneously display arbitrary different pixel patterns in all 32 cells.

Possible approaches:

1. Sliding Window / Viewport  
   - Only 8 actively “graphical” cells at a time (e.g., for a sprite).

2. Time Multiplexing (NOT ideal)  
   - Rapidly redefine CGRAM for cells just before writing them. But once you overwrite CGRAM for a previous cell, its appearance changes too. (LCD caches only character codes, not bitmaps.)

3. Pattern Compression  
   - Reuse identical glyph patterns across multiple cells; track duplicates so they point to the same CGRAM slot.

4. Partial Graphics + Text Hybrid  
   - Reserve, say, 4 glyphs for graphics region and 4 for custom icons.

---

## 9. Generalized Data Structures (Reuse Glyphs)

Below is a more advanced sketch that:
- Maintains a map from cell -> glyph pattern
- Deduplicates patterns (if identical, reuse CGRAM slot)
- Evicts least-recently-used pattern if >8 unique patterns needed

This still will change previously drawn cells if their pattern is evicted (inherent limitation) but attempts reuse.

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

struct GlyphPattern {
  byte rows[8]; // 5 LSBs used
};

struct Slot {
  bool used;
  GlyphPattern pat;
  unsigned long lastUse;
};

Slot slots[8];

GlyphPattern cellPatterns[2][16];  // Intended pattern per cell
int cellSlot[2][16];               // Which slot index currently assigned (-1 if none)

unsigned long useCounter = 0;

bool equalPattern(const GlyphPattern &a, const GlyphPattern &b) {
  for (int i=0;i<8;i++)
    if (a.rows[i] != b.rows[i]) return false;
  return true;
}

int findSlotForPattern(const GlyphPattern &pat) {
  // 1. Try reuse
  for (int s=0; s<8; s++) {
    if (slots[s].used && equalPattern(slots[s].pat, pat)) {
      slots[s].lastUse = ++useCounter;
      return s;
    }
  }
  // 2. Find free
  for (int s=0; s<8; s++) {
    if (!slots[s].used) {
      slots[s].used = true;
      slots[s].pat = pat;
      slots[s].lastUse = ++useCounter;
      lcd.createChar(s, (byte*)pat.rows);
      return s;
    }
  }
  // 3. Evict LRU
  int lru = 0;
  unsigned long best = slots[0].lastUse;
  for (int s=1; s<8; s++) {
    if (slots[s].lastUse < best) {
      best = slots[s].lastUse;
      lru = s;
    }
  }
  slots[lru].pat = pat;
  slots[lru].lastUse = ++useCounter;
  lcd.createChar(lru, (byte*)pat.rows);
  return lru;
}

void assignCell(int row, int col) {
  int slot = findSlotForPattern(cellPatterns[row][col]);
  cellSlot[row][col] = slot;
  lcd.setCursor(col, row);
  lcd.write((uint8_t)slot);
}

void clearAll() {
  for (int r=0;r<2;r++)
    for (int c=0;c<16;c++) {
      for (int i=0;i<8;i++) cellPatterns[r][c].rows[i] = 0;
      cellSlot[r][c] = -1;
    }
  for (int s=0;s<8;s++) {
    slots[s].used = false;
  }
  lcd.clear();
}

void setPixel(int x, int y) {
  if (x<0 || x>=80 || y<0 || y>=16) return;
  int col = x / 5;
  int row = y / 8;
  int bitPos = x % 5;
  int glyphRow = y % 8;
  int bitIndex = 4 - bitPos;

  byte mask = (1 << bitIndex);
  if (!(cellPatterns[row][col].rows[glyphRow] & mask)) {
    cellPatterns[row][col].rows[glyphRow] |= mask;
    assignCell(row, col);
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
  clearAll();

  // Example: plot a sine-ish wave across width
  for (int x=0;x<80;x++) {
    float fx = (float)x / 80.0 * 2 * PI;
    float fy = (sin(fx) * 0.5f + 0.5f) * 15.0f; // 0..15
    setPixel(x, (int)fy);
    delay(10); // visualize drawing
  }
}

void loop() {}
```

Note: Evictions cause earlier cells sharing that slot to update with new patterns (visual artifacts). For simple icons or limited dynamic regions, constrain the number of unique patterns simultaneously visible to ≤8.

---

## 10. Helper: Converting a 5x8 Visual Grid to Bytes

You can sketch a glyph with ‘X’ for a pixel:

```
X....
.X...
..X..
...X.
....X
.....
.....
.....
```

Convert each row (leftmost = bit 4):

Row string -> bits:
- X.... -> 10000 -> 0b10000 -> 16
- .X... -> 01000 -> 0b01000 -> 8
- ..X.. -> 00100 -> 4
- ...X. -> 00010 -> 2
- ....X -> 00001 -> 1
Remaining rows 0

---

## 11. Debugging Tips

- If characters show gibberish, confirm I2C address.
- If custom glyphs look mirrored horizontally, flip bit order (use `bitWrite()` with reversed index).
- Flicker: minimize calling `createChar()` repeatedly inside loops; batch updates.
- Avoid using the bottom row (row 7 inside glyph) if you plan to use underline cursor features.

---

## 12. Performance Considerations

- `createChar()` writes to LCD instruction interface; it's relatively slow (~40–50 µs per byte). Heavy dynamic updates reduce frame rate.
- For “animation” aim for small region + reuse patterns.
- Precompute frames if possible.

---

## 13. Common Patterns / Icons (5x8)

Battery (half-full):

```
11111  = 0b11111 = 31
10001  = 17
10111  = 23
10111  = 23
10111  = 23
10001  = 17
11111  = 31
00000  = 0
```

```cpp
byte batteryHalf[8] = {31,17,23,23,23,17,31,0};
lcd.createChar(1, batteryHalf);
lcd.write((uint8_t)1);
```

---

## 14. Quick Reference Cheat Sheet

Mapping:
x => col = x / 5
y => row = y / 8
cell-local col = x % 5 (bit index = 4 - localCol)
cell-local row = y % 8

Creating char:
lcd.createChar(slot, byteArrayOf8);

Displaying:
lcd.setCursor(col,row);
lcd.write(slot);

Limit:
Max simultaneously distinct custom patterns = 8.

---

## 15. Next Steps / Enhancements

- Implement `clearPixel(x,y)` (unset bit and possibly collapse identical patterns).
- Add double-buffering: build all glyphs first, then push changes.
- Build a tiny font overlay for micro text in a confined region.
- Implement sprite drawing (copy pattern arrays into cellPatterns quickly).

---

## 16. Troubleshooting Table

Issue: Nothing displays  
Cause: Wrong I2C address | Fix: Run I2C scanner.

Issue: Custom char looks shifted  
Cause: Bit order mismatch | Fix: Reverse bit indexing.

Issue: Earlier graphics morph when drawing new pixels  
Cause: CGRAM slot reuse/eviction | Fix: Constrain unique cells ≤8.

Issue: Flicker on animation  
Cause: Excessive createChar calls | Fix: Cache, reduce updates per frame.

---
