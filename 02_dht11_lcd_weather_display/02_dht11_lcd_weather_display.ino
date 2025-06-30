#include <LiquidCrystal.h>
#include <DHT.h>

// DHT sensor settings
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// LCD pin connections to Arduino
const int RS = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2); // Initialize 16x2 LCD

  // Display startup message
  lcd.setCursor(0, 0);
  lcd.print("Reading sensor...");
  lcd.setCursor(0, 1);
  lcd.print("Please wait");
  delay(2000); // Delay for readability
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if the readings are valid
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor error");
    lcd.setCursor(0, 1);
    lcd.print("Check DHT11");
    delay(2000);
    return;
  }

  // Output to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1); // 1 decimal place
  lcd.print((char)223);      // Degree symbol
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity, 1);
  lcd.print("%");

  delay(2000); // Update every 2 seconds
}
