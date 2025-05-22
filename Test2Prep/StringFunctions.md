---

# Arduino Serial String Parsing: Reading, Searching, and Extracting Data

#### Demos for reading user data from the Serial Monitor, determining string length, searching for characters, and extracting substrings and numbers.

---

## 1. Determining the Length and Character Position in Input (Section 9.1.1)

---

### Circuit Link

[https://www.tinkercad.com/things/6QEe7axDLEj-design-911](https://www.tinkercad.com/things/6QEe7axDLEj-design-911)

---

### Example Code

```cpp
int inputLength, stringLength; 
char readChar;
String userIn; // Use Arduino's String type

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    userIn = Serial.readStringUntil('\n');
    userIn.trim();
    inputLength = userIn.length();
    if (userIn.length() > 3) {
      readChar = userIn.charAt(3);
    } else {
      readChar = '\0';
    }
    stringLength = userIn.length();
    Serial.print("Input Length: ");
    Serial.println(inputLength);
    Serial.print("User Input: ");
    Serial.println(userIn);
    Serial.print("String Length: ");
    Serial.println(stringLength);
    Serial.print("Character at Index 3: ");
    Serial.println(readChar);
  }
}
```

---

### Notes

- Reads a string from Serial Monitor, trims whitespace, calculates length, and finds the character at position 3 (4th character, zero-based).
- Prints all info to Serial Monitor when a new line is sent.
- Handles strings of any length.
- Character at index 3 will be empty if input is too short.

---

## 2. Using Substring and Finding the Position of Data (Section 9.1.2)

---

### Circuit Link

[https://www.tinkercad.com/things/4m8qvyViJkS-design-912](https://www.tinkercad.com/things/4m8qvyViJkS-design-912)

---

### Example Code

```cpp
int inputLength, startPos; 
char readChar;
String userIn, partString;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    userIn = Serial.readStringUntil('\n');
    userIn.trim();
    inputLength = userIn.length();
    partString = userIn.substring(2,5);
    startPos = userIn.indexOf("5");
    Serial.print("User input:");
    Serial.println(userIn);
    Serial.print("This is a substring:");
    Serial.println(partString);
    Serial.print("Position:");
    Serial.println(startPos);
  }
}
```

---

### Notes

- `substring(start, end)` extracts characters from index `start` up to but not including `end`.
- `indexOf("5")` returns the position of the first '5' in the input string (returns -1 if not found).
- You can change what to search for by changing the string in `indexOf()`.

---

## 3. Using indexOf() to Get Position of a Substring (Section 9.2.1)

---

### Circuit Link

[https://www.tinkercad.com/things/7DzMk4N7V5T-design-92](https://www.tinkercad.com/things/7DzMk4N7V5T-design-92)

---

### Example Code

```cpp
int startPos, endPos; 
String userIn, partString;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    userIn = Serial.readStringUntil('\n');
    Serial.println(userIn);
    userIn.trim();
    endPos = userIn.indexOf(");");
    startPos = userIn.indexOf("On(");
    if((startPos > -1)&&(endPos > startPos)){
      Serial.print("StartPos =");
      Serial.println(startPos);
      Serial.print("endPos =");
      Serial.print(endPos);
    } else {
      Serial.println("Nothing to switch on");
    }
  }
}
```

---

### Notes

- `indexOf("On(")` looks for the substring "On(" and returns its starting index.
- `indexOf(");")` looks for the substring ");" and returns its starting index.
- If either substring is missing, prints "Nothing to switch on".
- This is useful for parsing command-like inputs (e.g., `Turn On(5); the light`).

---

### Understanding `indexOf()` in Arduino Strings

- Returns the index (zero-based) of the substring or -1 if not found.
- Case-sensitive and finds the first occurrence.
- Useful for searching for commands or patterns inside serial input.

---

## 4. Extracting Numbers Using Substring and indexOf() (Section 9.2.2)

---

### Circuit Link

[https://www.tinkercad.com/things/iguccydT2T1-design-922](https://www.tinkercad.com/things/iguccydT2T1-design-922)

---

### Example Code

```cpp
int startPos, endPos; 
String userIn, partString, stringPiece;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    userIn = Serial.readStringUntil('\n');
    Serial.println(userIn);
    userIn.trim();
    endPos = userIn.indexOf(");");
    startPos = userIn.indexOf("On(");
    if((startPos > -1)&&(endPos > startPos)){
      Serial.print("StartPos =");
      Serial.println(startPos);
      startPos = startPos + 3;  // Move past "On("
      Serial.print("endPos =");
      Serial.println(endPos);
      stringPiece = userIn.substring(startPos, endPos);
      Serial.print("stringPiece = ");
      Serial.println(stringPiece);
    } else {
      Serial.println("Nothing to switch on");
    }
  }
}
```

---

### Notes

- This code finds a number or value inside parentheses in commands like `On(9);`.
- `startPos` is incremented to skip past "On(" and get only the value inside the brackets.
- `substring(startPos, endPos)` extracts just what's between the parentheses.
- Useful for command parsing in simple serial protocols.

---
