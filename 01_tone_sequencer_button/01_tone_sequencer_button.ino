// Pins for connected LEDs (representing notes from C4 to B4)
constexpr int LED_PINS[] = {13, 12, 11, 10, 9, 8, 7};

// Frequencies of notes C4 to B4 in Hertz
constexpr int NOTE_FREQS[] = {
  262, // C4
  294, // D4
  330, // E4
  349, // F4
  392, // G4
  440, // A4
  494  // B4
};

// Pin connected to the button
constexpr int BUTTON_PIN = 2;

// Pin connected to the passive buzzer
constexpr int BUZZER_PIN = 4;

// Number of LEDs / notes
constexpr int sizeOfArray = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

// Variable to hold the current state of the button
int currentButtonState;

void setup() {
  // Set all LED pins as OUTPUT
  for (int pin : LED_PINS) {
    pinMode(pin, OUTPUT);
  }

  // Set the buzzer pin as OUTPUT
  pinMode(BUZZER_PIN, OUTPUT);

  // Set the button pin as INPUT
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  // Read the current state of the button
  currentButtonState = digitalRead(BUTTON_PIN);

  // Simple debounce: wait 50 ms if button is pressed
  if (currentButtonState == HIGH) delay(50);

  // Check again if the button is still pressed after the debounce delay
  if (digitalRead(BUTTON_PIN) == HIGH) {
    // Loop through each note and corresponding LED
    for (int i = 0; i < sizeOfArray; i++) {
      digitalWrite(LED_PINS[i], HIGH);        // Turn on the corresponding LED
      tone(BUZZER_PIN, NOTE_FREQS[i]);        // Play the corresponding note
      delay(1000);                            // Play the note for 1 second
      digitalWrite(LED_PINS[i], LOW);         // Turn off the LED
      noTone(BUZZER_PIN);                     // Stop the sound
      delay(100);                             // Short pause before the next note
    }
  }
}
