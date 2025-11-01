#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// -------- DHT Settings --------
#define DHTPIN 2
#define DHTTYPE DHT11    // Change to DHT22 if using that
DHT dht(DHTPIN, DHTTYPE);

// -------- LCD --------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// -------- Custom Emoji Characters --------
// Thermometer 🌡️
byte thermo[8] = {
  B00100,
  B01010,
  B01010,
  B01010,
  B01110,
  B01110,
  B01110,
  B00100
};

// Drop 💧
byte drop[8] = {
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B01110,
  B00000
};

// Smiley 😀
byte smiley[8] = {
  B00000,
  B01010,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};

// Skull 💀 (alert)
byte skull[8] = {
  B01110,
  B10101,
  B11111,
  B11111,
  B01110,
  B01010,
  B10101,
  B00000
};

void setup() {
  lcd.init();
  lcd.backlight();
  dht.begin();

  // Load emojis into custom memory slots
  lcd.createChar(0, thermo);
  lcd.createChar(1, drop);
  lcd.createChar(2, smiley);
  lcd.createChar(3, skull);

  // Startup screen
  lcd.setCursor(0, 0);
  lcd.print(" DHT Live Data ");
  lcd.setCursor(0, 1);
  lcd.write(byte(2)); // smiley
  lcd.print(" Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();  // Celsius

  // Handle sensor error
  if (isnan(h) || isnan(t)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(3)); // skull icon
    lcd.print(" Sensor Error  ");
    lcd.setCursor(0, 1);
    lcd.print("Check wiring   ");
    delay(1500);
    return;
  }

  // ---------- Line 1: Temperature ----------
  lcd.setCursor(0, 0);
  lcd.write(byte(0));      // 🌡️ icon
  lcd.print(" Temp: ");
  lcd.print(t, 1);
  lcd.print((char)223);    // degree symbol
  lcd.print("C  ");

  // ---------- Line 2: Humidity ----------
  lcd.setCursor(0, 1);
  lcd.write(byte(1));      // 💧 icon
  lcd.print(" Humi: ");
  lcd.print(h, 1);
  lcd.print("%  ");

  // Add emoji status at the end (column 15)
  lcd.setCursor(15, 0);
  if (t >= 20 && t <= 30 && h >= 30 && h <= 70) {
    lcd.write(byte(2));    // 😀 if in comfort range
  } else {
    lcd.write(byte(3));    // 💀 skull if out of range
  }

  delay(1000);
}
