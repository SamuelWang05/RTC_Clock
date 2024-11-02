#include "Arduino.h"
#include "LiquidCrystal.h"
#include "RtcDS1302.h"
#include "DHT11.h"

// Bunch of setup stuff for RTC
ThreeWire myWire(3, 4, 2);
RtcDS1302<ThreeWire> Rtc(myWire);

// Bunch of setup stuff for LCD Screen
const int rs = 12, en = 13, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd{ rs, en, d4, d5, d6, d7 };

// Setup for DHT module
DHT11 dht11(6); // Digital pin 6

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);

  // Fix date/time
  Rtc.Begin();

  // Only needed once for initial setup -- can comment out after
  
  Serial.println("Setting up clock...");
  RtcDateTime currTime = RtcDateTime(__DATE__, __TIME__);
  Rtc.SetDateTime(currTime);

  delay(2000); // DHT 11 needs to stabilize
}

void loop() {
  int temp = dht11.readTemperature();
  int humidity = dht11.readHumidity();

  temp = (temp * 1.8) + 32; // Convert C to F

  RtcDateTime now = Rtc.GetDateTime();  // Gets various data about day and time

  // Set cursor for LCD to col 0, line 0 (0-based indexing)
  lcd.setCursor(0, 0);

  // Print time data
  lcd.print(now.Hour());
  lcd.print(":");
  if(now.Minute() >= 10) {
    lcd.print(now.Minute());
  } else {
    lcd.print("0");
    lcd.print(now.Minute());
  }
  

  lcd.print("  ");
  lcd.print(now.Month());
  lcd.print("/");
  lcd.print(now.Day());
  lcd.print("/");
  lcd.print(now.Year());

  // Print temp, humidity data
  lcd.setCursor(0,1);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print("F   H:");
  lcd.print(humidity);
  lcd.print("%");

  delay(1000);
}
