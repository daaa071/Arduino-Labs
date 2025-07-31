#include "Adafruit_Keypad.h"
#include <LiquidCrystal.h>

// LCD pin configuration (adjust as needed)
constexpr int LCD_RS = 7;
constexpr int LCD_EN = 6;
constexpr int LCD_D4 = 5;
constexpr int LCD_D5 = 4;
constexpr int LCD_D6 = 3;
constexpr int LCD_D7 = 2;

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Keypad configuration
constexpr byte ROWS = 4;
constexpr byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Keypad pin connections (rows and columns)
byte rowPins[ROWS] = {9, 8, A4, A5};
byte colPins[COLS] = {10, 11, 12, 13};

Adafruit_Keypad keypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// PIN code configuration
const String kCorrectPin = "1234";
String enteredPin = "";

constexpr int kMaxAttempts = 3;
int attempts = 0;
bool isLocked = false;

// Output pins (e.g., LEDs, relays)
constexpr int pinGreen = A1; // Access Granted indicator
constexpr int pinRed = A0;   // System active indicator

void setup() {
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);

  keypad.begin();
  lcd.begin(16, 2);
  lcd.print("Enter PIN:");

  Serial.begin(9600);
}

void loop() {
  keypad.tick();
  digitalWrite(pinRed, HIGH); // System indicator ON

  if (isLocked) {
    lcd.setCursor(0, 1);
    lcd.print("LOCKED         ");
    return;
  }

  while (keypad.available()) {
    keypadEvent e = keypad.read();
    char key = static_cast<char>(e.bit.KEY);

    if (e.bit.EVENT == KEY_JUST_PRESSED) {
      Serial.print(key);
      Serial.println(" pressed");

      if (key == '#') {
        checkPin();
      } else if (key == '*') {
        clearPinInput();
      } else if (isdigit(key) && enteredPin.length() < 4) {
        enteredPin += key;
        lcd.setCursor(enteredPin.length() - 1, 1);
        lcd.print('*');  // Masked input
      }
    }
  }

  delay(10);
}

// Verifies the entered PIN against the correct one
void checkPin() {
  lcd.setCursor(0, 1);
  if (enteredPin == kCorrectPin) {
    lcd.print("Access Granted ");
    Serial.println("Access Granted");

    attempts = 0;

    digitalWrite(pinRed, LOW);
    digitalWrite(pinGreen, HIGH);

    delay(3000);
    digitalWrite(pinGreen, LOW);
    resetSystem();
  } else {
    attempts++;
    lcd.print("Access Denied  ");
    Serial.println("Access Denied");

    delay(2000);
    resetSystem();

    if (attempts >= kMaxAttempts) {
      isLocked = true;
      lcd.setCursor(0, 1);
      lcd.print("LOCKED         ");
    }
  }
}

// Clears the entered PIN input and updates display
void clearPinInput() {
  enteredPin = "";
  lcd.setCursor(0, 1);
  lcd.print("Cleared        ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

// Resets the system to accept new PIN
void resetSystem() {
  enteredPin = "";

  lcd.clear();
  lcd.print("Enter PIN:");
}
