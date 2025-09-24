#include <LiquidCrystal.h>

/**
 * @brief Configuration for the LED matrix (4x4)
 */
namespace MatrixConfig {
  constexpr int ROW_COUNT = 4;
  constexpr int COL_COUNT = 4;

  // Row pins (anodes) — R3, R4, R5, R6
  constexpr int rowPins[ROW_COUNT] = {10, 13, 12, 11};

  // Column pins (cathodes) — C3, C4, C5, C6
  constexpr int colPins[COL_COUNT] = {A1, A3, A0, A2};
}

/**
 * @brief LCD configuration (16x2)
 */
namespace LCDConfig {
  constexpr int RS = 7;
  constexpr int EN = 6;
  constexpr int D4 = 5;
  constexpr int D5 = 4;
  constexpr int D6 = 3;
  constexpr int D7 = 2;
  constexpr int WIDTH = 16;
  constexpr int HEIGHT = 2;
}

/**
 * @brief Button configuration
 */
namespace ButtonConfig {
  constexpr int pin = 8;
}

/**
 * @brief Circle pattern (4x4 LED matrix)
 */
constexpr byte circlePattern[MatrixConfig::ROW_COUNT] = {
  B0110,  // Row 0: C4, C5
  B1111,  // Row 1: C3, C4, C5, C6
  B1111,  // Row 2: C3, C4, C5, C6
  B0110   // Row 3: C4, C5
};

/**
 * @brief Game state machine
 */
enum class GameState {
  WaitRandomDelay,
  ShowCircle,
  WaitForButton,
  ShowResult
};

// --- Global variables ---
LiquidCrystal lcd(
  LCDConfig::RS, LCDConfig::EN,
  LCDConfig::D4, LCDConfig::D5,
  LCDConfig::D6, LCDConfig::D7
);

GameState currentState = GameState::WaitRandomDelay;

unsigned long stateStartTime = 0;
unsigned long circleShownTime = 0;
unsigned long reactionTime = 0;

unsigned long lastMatrixUpdate = 0;
int currentRow = 0;

// === Setup ===
void setup() {
  lcd.begin(LCDConfig::WIDTH, LCDConfig::HEIGHT);
  lcd.print("Reaction Game");

  pinMode(ButtonConfig::pin, INPUT_PULLUP);

  for (int i = 0; i < MatrixConfig::ROW_COUNT; ++i) {
    pinMode(MatrixConfig::rowPins[i], OUTPUT);
    pinMode(MatrixConfig::colPins[i], OUTPUT);
  }

  delay(2000);
  lcd.clear();

  stateStartTime = millis();
}

// === Main loop ===
void loop() {
  // LED matrix refresh (≈ 125 Hz)
  if (millis() - lastMatrixUpdate >= 2) {
    updateMatrix();
    lastMatrixUpdate = millis();
  }

  // Game state machine logic
  switch (currentState) {
    case GameState::WaitRandomDelay:
      handleWaitRandomDelay();
      break;

    case GameState::ShowCircle:
      handleShowCircle();
      break;

    case GameState::WaitForButton:
      handleWaitForButton();
      break;

    case GameState::ShowResult:
      handleShowResult();
      break;
  }
}

// === FSM Handlers ===

void handleWaitRandomDelay() {
  lcd.setCursor(0, 0);
  lcd.print("Wait for circle...");

  constexpr unsigned long minDelayMs = 1000;
  constexpr unsigned long maxDelayMs = 5000;

  if (millis() - stateStartTime >= random(minDelayMs, maxDelayMs)) {
    circleShownTime = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press NOW!");

    currentState = GameState::ShowCircle;
    stateStartTime = millis();
  }
}

void handleShowCircle() {
  // Transition immediately to waiting for button (but after displaying text)
  if (millis() - stateStartTime >= 100) { // Prevent accidental early press
    currentState = GameState::WaitForButton;
  }
}

void handleWaitForButton() {
  if (digitalRead(ButtonConfig::pin) == LOW) {
    reactionTime = millis() - circleShownTime;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reaction Time:");

    lcd.setCursor(0, 1);
    lcd.print(reactionTime / 1000.0, 3);
    lcd.print(" s");

    currentState = GameState::ShowResult;
    stateStartTime = millis();
  }
}

void handleShowResult() {
  constexpr unsigned long resultDisplayTime = 3000;
  if (millis() - stateStartTime >= resultDisplayTime) {
    lcd.clear();
    currentState = GameState::WaitRandomDelay;
    stateStartTime = millis();
  }
}

// === LED Matrix Multiplexing ===

void updateMatrix() {
  const byte* img = (currentState == GameState::ShowCircle ||
                     currentState == GameState::WaitForButton)
                     ? circlePattern : nullptr;

  // Turn off all rows and columns
  for (int i = 0; i < MatrixConfig::ROW_COUNT; ++i) {
    digitalWrite(MatrixConfig::colPins[i], HIGH); // Cathodes OFF
    digitalWrite(MatrixConfig::rowPins[i], LOW);  // Anodes OFF
  }

  if (img) {
    int row = currentRow;

    // Set columns for current row
    for (int col = 0; col < MatrixConfig::COL_COUNT; ++col) {
      bool pixelOn = bitRead(img[row], 3 - col);
      digitalWrite(MatrixConfig::colPins[col], pixelOn ? LOW : HIGH);
    }

    digitalWrite(MatrixConfig::rowPins[row], HIGH); // Activate row

    delayMicroseconds(1000); // Short pulse for visibility
    digitalWrite(MatrixConfig::rowPins[row], LOW);  // Deactivate row
  }

  currentRow = (currentRow + 1) % MatrixConfig::ROW_COUNT;
}
