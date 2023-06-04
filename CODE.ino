#include "DHT.h"
#include <Adafruit_BMP085.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Adjust pin numbers according to your LCD connections

#define BUZZER_PIN D8
#define DHTTYPE DHT11
#define AOUT_PIN A0
#define Light A0 // Change to A0 for LDR sensor
#define rainDigital D2
#define BMP_SDA D4
#define BMP_SCL D3
#define MOTION_PIN D7

uint8_t DHTPin = D5;
DHT dht(DHTPin, DHTTYPE);
Adafruit_BMP085 bmp;

float Temperature;
float Humidity;
float Temp_Fahrenheit;
int moisture = 0;
int value = 0;
int val = 0;
int rainDigitalVal = 0;
int motion = 0;
float pressure = 0;
int light = 0;

const int RAIN_THRESHOLD = 50; // Adjust the threshold value as needed

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(rainDigital, INPUT);
  pinMode(MOTION_PIN, INPUT);
  dht.begin();
  bmp.begin();

  lcd.begin(16, 2); // Adjust the dimensions of your LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
}

void loop() {
  Humidity = dht.readHumidity();
  Temperature = dht.readTemperature();
  Temp_Fahrenheit = dht.readTemperature(true);
  rainDigitalVal = digitalRead(rainDigital);
  moisture = analogRead(AOUT_PIN);
  moisture = map(moisture, 0, 1023, 0, 100);

  motion = digitalRead(MOTION_PIN);
  pressure = bmp.readPressure() / 100.0;
  light = analogRead(Light);

  if (isnan(Humidity) || isnan(Temperature) || isnan(Temp_Fahrenheit)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(Humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(Temperature);
  Serial.print(F("°C "));
  Serial.print(Temp_Fahrenheit);
  Serial.print(F("°F "));
  Serial.print(F("Pressure: "));
  Serial.print(pressure);
  Serial.print(F(" hPa"));
  Serial.print(F(" Light: "));
  Serial.println(light);

  lcd.setCursor(7, 0);
  lcd.print(Temperature);
  lcd.print(" C");

  lcd.setCursor(11, 1);
  lcd.print(Humidity);
  lcd.print(" %");

  lcd.setCursor(0, 1);
  lcd.print("Moist: ");
  lcd.print(moisture);
  lcd.print("%  Rain: ");
  lcd.print(rainDigitalVal == 1 ? "Yes" : "No");

  lcd.setCursor(0, 0);
  lcd.print("Motion: ");
  lcd.print(motion == 1 ? "Detected" : "Not Detected");

  lcd.setCursor(0, 1);
  lcd.print("Pressure: ");
  lcd.print(pressure);
  lcd.print(" hPa");

  lcd.setCursor(8, 0);
  lcd.print("Light: ");
  lcd.print(light);

  if (rainDigitalVal == 1 || moisture > RAIN_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);
    lcd.display(); // Turn on the LCD display
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    lcd.noDisplay(); // Turn off the LCD display
  }

  if (motion == 1) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(1000);
}
