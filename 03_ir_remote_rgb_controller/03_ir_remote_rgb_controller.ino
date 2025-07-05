#include <IRremote.h>

constexpr int recv_pin = 2;           // Pin for receiving IR signal
constexpr int bluePin = 5;            // Pin for blue LED
constexpr int redPin = 9;             // Pin for red LED
constexpr int greenPin = 10;          // Pin for green LED

IRrecv irrecv(recv_pin);              // Initialize IR receiver
decode_results results;               // Variable to store the decoded results

constexpr uint32_t CH_ = 0xFFA25D;    // Code for CH- button
constexpr uint32_t CH = 0xFF629D;     // Code for CH button
constexpr uint32_t CH_PLUS = 0xFFE21D;// Code for CH+ button
constexpr uint32_t PREV = 0xFF22DD;   // Code for << button
constexpr uint32_t NEXT = 0xFF02FD;   // Code for >> button
constexpr uint32_t PAUSE = 0xFFC23D;  // Code for >|| button
constexpr uint32_t MINUS = 0xFFE01F;  // Code for - button
constexpr uint32_t PLUS = 0xFFA857;   // Code for + button
constexpr uint32_t EQ = 0xFF906F;     // Code for = button
constexpr uint32_t _100_PLUS = 0xFF9867;  // Code for 100+ button
constexpr uint32_t _200_PLUS = 0xFFB04F; // Code for 200+ button
constexpr uint32_t ZERO = 0xFF6897;   // Code for 0 button
constexpr uint32_t ONE = 0xFF30CF;    // Code for 1 button
constexpr uint32_t TWO = 0xFF18E7;    // Code for 2 button
constexpr uint32_t THREE = 0xFF7A85;  // Code for 3 button
constexpr uint32_t FOUR = 0xFF10EF;   // Code for 4 button
constexpr uint32_t FIVE = 0xFF38C7;   // Code for 5 button
constexpr uint32_t SIX = 0xFF5AA5;    // Code for 6 button
constexpr uint32_t SEVEN = 0xFF42BD;  // Code for 7 button
constexpr uint32_t EIGHT = 0xFF4AB5;  // Code for 8 button
constexpr uint32_t NINE = 0xFF52AD;   // Code for 9 button
constexpr uint32_t EMPTY_CODE = 0xFFFFFFFF; // Code to ignore (if no signal or "fake" signal)

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();  // Start the IR receiver
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  if (irrecv.decode(&results)) {   // If IR signal is received
    long int decCode = results.value;   // Decoded value of the IR signal
    
    // Ignore the FFFFFFF code (if no button pressed or "fake" signal)
    if (decCode == EMPTY_CODE) {
      irrecv.resume();  // Wait for the next IR signal
      return;
    }

    Serial.println(decCode, HEX);    // Print the value in hexadecimal

    // Perform an action based on the pressed button
    switch (decCode) {
      case CH_:
        animateColor(255, 0, 0);  // Animate the red color appearance
        break;
      case CH:
        animateColor(0, 255, 0);  // Animate the green color appearance
        break;
      case CH_PLUS:
        animateColor(0, 0, 255);  // Animate the blue color appearance
        break;
      case PREV:
        animatePrev();  // "<<" (PREV) button: Pulsating light effect that fades in and out.
        break;
      case NEXT:
        animateNext();  // ">>" (NEXT) button: Quick color pulsation between cyan and white.
        break;
      case PAUSE:  
        animatePause(); // ">=" (PAUSE) button: Smooth transition from red to blue.
        break;
      case MINUS:
        animateMinus(); // "-" (MINUS) button: Gradual dimming, reducing brightness.
        break;
      case PLUS:
        animatePlus();  // "+" (PLUS) button: Gradual increase in brightness.
        break;
      case EQ:
        animateEQ();    // "=" (EQ) button: White light burst effect for an explosion-like effect.
        break;
      case _100_PLUS:
        animateColor100Plus(); // Animate color transition between red, orange, etc.
        break;
      case _200_PLUS:
        animateColor200Plus(); // Animation for 200+
        break;
      case ZERO:
        setColor(0, 0, 0); // Turn off the light for 0
        break;
      case ONE:
        setColor(255, 0, 0); // Red for 1
        break;
      case TWO:
        setColor(255, 50, 50); // Ochre for 2
        break;
      case THREE:
        setColor(255, 200, 0); // Orange for 3
        break;
      case FOUR:
        setColor(255, 255, 0); // Yellow for 4
        break;
      case FIVE:
        setColor(0, 128, 0); // Dark Green for 5
        break;
      case SIX:
        setColor(0, 255, 255); // Turquoise for 6
        break;
      case SEVEN:
        setColor(0, 0, 255); // Blue for 7
        break;
      case EIGHT:
        setColor(255, 0, 255); // Purple for 8
        break;
      case NINE:
        setColor(255, 255, 255); // White for 9
        break;
      default:
        setColor(0, 0, 0); // Turn off if button not found
        break;
    }
    
    irrecv.resume();  // Wait for the next IR signal
  }
}

// Function to set the RGB color
void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

// Animation for gradual color transition from 0 to maximum (0 - 255)
void animateColor(int red, int green, int blue) {
  for (int i = 0; i <= 255; i++) {
    setColor(i * red / 255, i * green / 255, i * blue / 255);
    delay(10); // Animation speed
  }
}

void animatePrev() {
  for (int i = 0; i <= 255; i++) {
    setColor(255 - i, 255 - i, 255 - i); // Gradual fading (black and white)
    delay(10);
  }
  for (int i = 255; i >= 0; i--) {
    setColor(255 - i, 255 - i, 255 - i); // Return to initial brightness
    delay(10);
  }
}

void animateNext() {
  for (int i = 0; i < 5; i++) {
    setColor(0, 255, 255); // Cyan
    delay(100);
    setColor(255, 255, 255); // White flash
    delay(100);
  }
}

void animatePause() {
  for (int i = 0; i <= 255; i++) {
    setColor(255 - i, 0, i); // Transition from red to blue
    delay(5);
  }
}

void animateMinus() {
  for (int i = 255; i >= 0; i--) {
    setColor(i, i, i); // Gradual dimming
    delay(5);
  }
}

void animatePlus() {
  for (int i = 0; i <= 255; i++) {
    setColor(i, i, i); // Gradual increase in brightness
    delay(5);
  }
}

void animateEQ() {
  for (int i = 0; i < 3; i++) {
    setColor(255, 255, 255); // White explosion
    delay(200);
    setColor(0, 0, 0);       // Turn off
    delay(200);
  }
}

// Animation for color sequence on the 100+ button
void animateColor100Plus() {
  int colors[5][3] = {
    {255, 0, 0},    // Red
    {255, 50, 50},  // Orange
    {255, 255, 0},  // Yellow
    {0, 255, 0},    // Green
    {0, 0, 255}     // Blue
  };

  for (int i = 0; i < 5; i++) {
    setColor(colors[i][0], colors[i][1], colors[i][2]);
    delay(500); // Duration for each color
  }
}

// Animation for color sequence on the 200+ button
void animateColor200Plus() {
  int colors[5][3] = {
    {255, 0, 0},    // Red
    {255, 50, 0},   // Orange
    {255, 255, 0},  // Yellow
    {0, 255, 0},    // Green
    {0, 0, 255}     // Blue
  };

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j <= 255; j++) {
      setColor(colors[i][0] * j / 255, colors[i][1] * j / 255, colors[i][2] * j / 255);
      delay(5); // Slow transition
    }
    for (int j = 255; j >= 0; j--) {
      setColor(colors[i][0] * j / 255, colors[i][1] * j / 255, colors[i][2] * j / 255);
      delay(5); // Fade back to 0
    }
  }
}
