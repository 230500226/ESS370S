---

# Arduino Random Numbers & Array Manipulation

#### Iteration is bad!
#### Demos for random number generation and manipulating integer arrays (fill, print, shift left/right).

---

## 1. Generating Random Numbers (Section 8.1)

---

### Circuit Link

[https://www.tinkercad.com/things/0OzbR6ZOkqN-design-81](https://www.tinkercad.com/things/0OzbR6ZOkqN-design-81)

---

### Example Code

```cpp
long randVal;
void setup()
{
  pinMode(A5, INPUT);
  Serial.begin(2000000);
  randVal = analogRead(A5);
  randomSeed(randVal); // Seed PRNG using noisy analog input
}

void loop()
{
  randVal = random(20);
  Serial.println(randVal);
  delay(400);	
}
```

---

### Notes

- **randomSeed(randVal)**: Seeds the random number generator with a noisy analog pin to ensure different sequences every run.
- **random(20)**: Generates pseudo-random integer values between 0 and 19 (inclusive).
- The analog pin (A5) should be left unconnected for maximum noise/randomness.

---

## 2. LED Flashing in a Random Fashion (Section 8.2)

---

### Circuit Link

[https://www.tinkercad.com/things/0YLCV267Irc-design-82](https://www.tinkercad.com/things/0YLCV267Irc-design-82)

---

### Example Code

```cpp
long randVal;
void setup()
{
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(3, INPUT);
  pinMode(A5, INPUT);
  Serial.begin(2000000);
  randVal = analogRead(A5);
  randomSeed(randVal);
}

void loop()
{
  randVal = random(255);
  analogWrite(3, randVal);

  randVal = random(255);
  analogWrite(5, randVal);

  randVal = random(255);
  analogWrite(6, randVal);

  delay(400);
}
```

---

### Notes

- Each LED (on pins 3, 5, 6) receives a new random PWM value (0–254) every 400ms.
- Creates a "twinkling" or random flashing effect.

---

## 3. Fill Up Array with 10 Values (Section 8.3)

---

### Circuit Link

[https://www.tinkercad.com/things/c6xtDkaG0QY-design-83](https://www.tinkercad.com/things/c6xtDkaG0QY-design-83)

---

### Example Code

```cpp
long randVal;
int intArray[10];

void setup()
{
  pinMode(3, INPUT);
  pinMode(6, INPUT);
  pinMode(A5, INPUT);
  Serial.begin(2000000);
  randVal = analogRead(A5);
  randomSeed(randVal);
}

void loop()
{
  fillArrayWithRandom();
  printArray();
  delay(400);
}

void fillArrayWithRandom() {
  for (int index = 0; index < (sizeof(intArray)/2); index++) {
    intArray[index] = random(256);
  }
}

void printArray() {
  for (int index = 0; index < (sizeof(intArray)/2); index++) {
    Serial.print(index);
    Serial.print("]=");
    Serial.println(intArray[index]);
  }
}
```

---

### Notes

- `sizeof(intArray)/2` gives the number of elements (10), since `int` is 2 bytes on Arduino.
- Array is filled with random numbers [0–255] and printed to Serial Monitor.

---

## 4. Shift Left Info in Array (Section 8.4)

---

### Circuit Link

[https://www.tinkercad.com/things/eHPn6KdYazb-design-84](https://www.tinkercad.com/things/eHPn6KdYazb-design-84)

---

### Example Code

```cpp
long randVal;
int intArray[10];

void setup() {
  pinMode(3, INPUT);
  pinMode(6, INPUT);
  pinMode(A5, INPUT);
  Serial.begin(9600);
  randVal = analogRead(A5);
  randomSeed(randVal);
  fillArrayWithRandom();
}

void loop() {
  printArray();
  shiftArray();
  delay(400);
}

void fillArrayWithRandom() {
  for(int index = 0; index < (sizeof(intArray)/2); index++) {
    intArray[index] = random(255); // 0-254
  }
}

void printArray() {
  for(int index = 0; index < (sizeof(intArray)/2); index++) {
    Serial.print(" ");
    Serial.print(intArray[index]);
  }
  Serial.println();
}

void shiftArray() {
  int temp = intArray[0]; // Store first element
  for(int index = 0; index < (sizeof(intArray)/2 - 1); index++) {
    intArray[index] = intArray[index + 1];
  }
  intArray[sizeof(intArray)/2 - 1] = temp; // Move first element to end
}
```

---

### Notes

- `shiftArray()` shifts all elements left, moving the first element to the end (circular shift).
- Array is printed before each shift.
- No data loss or duplication.

---

## 5. Shift Right Info in Array (Section 8.5)

---

### Circuit Link

[https://www.tinkercad.com/things/6uZsRc7z3wT-design-85](https://www.tinkercad.com/things/6uZsRc7z3wT-design-85)

---

### Example Code

```cpp
long randVal;
int intArray[10];

void setup() {
  pinMode(3, INPUT);
  pinMode(6, INPUT);
  pinMode(A5, INPUT);
  Serial.begin(9600);
  randVal = analogRead(A5);
  randomSeed(randVal);
  fillArrayWithRandom();
}

void loop() {
  printArray();
  shiftArrayRight();
  delay(400);
}

void fillArrayWithRandom() {
  for(int index = 0; index < (sizeof(intArray)/2); index++) {
    intArray[index] = random(255); // 0-254
  }
}

void printArray() {
  for(int index = 0; index < (sizeof(intArray)/2); index++) {
    Serial.print(" ");
    Serial.print(intArray[index]);
  }
  Serial.println();
}

void shiftArrayRight() {
  int temp = intArray[sizeof(intArray)/2 - 1];
  for(int index = (sizeof(intArray)/2 - 2); index > -1; index--) {
    intArray[index + 1] = intArray[index];
  }
  intArray[0] = temp;
}
```

---

### Notes

- `shiftArrayRight()` shifts all elements right, moving the last element to the front (circular shift).
- The loop runs `(N-1)` times for an array of N elements (here, 9 times for 10 elements).
- Useful for rotating array data in-place.

---

## Key Concepts

- **Random Number Generation:** Use `randomSeed()` and `random()` for unpredictable values.
- **Array Manipulation:** Use for-loops and shifting for rotating arrays.
- **Memory Size:** On Arduino, `sizeof(int)` is 2 bytes. `sizeof(intArray)/2` gives number of elements in an `intArray`.
- **Iteration:** Filling, printing, and shifting arrays are common Arduino tasks.

---
