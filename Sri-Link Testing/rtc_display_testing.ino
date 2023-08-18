#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"

RTC_DS3231 rtc;

char days[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire, -1);

void setup() {
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  if (! rtc.begin()) {
    Serial.println("Could not find RTC! Check circuit.");
    while (1);
  }

  rtc.adjust(DateTime(__DATE__, __TIME__));
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print("RTC CLOCK");
  display.display();
  delay(5000);
}

void loop() {
  DateTime now = rtc.now();

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(75, 0);
  display.println(now.second(), DEC);
  display.setTextSize(2);
  display.setCursor(25, 0);
  display.println(":");
  display.setTextSize(2);
  display.setCursor(65, 0);
  display.println(":");
  display.setTextSize(2);
  display.setCursor(40, 0);
  display.println(now.minute(), DEC);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(now.hour(), DEC);
  display.setTextSize(2);
  display.setCursor(0, 25);
  display.println(now.day(), DEC);
  display.print(days[now.dayOfTheWeek()]);
  display.setTextSize(2);
  display.setCursor(20, 25);
  display.println("-");
  display.setTextSize(2);
  display.setCursor(35, 25);
  display.println(now.month(), DEC);
  display.setTextSize(2);
  display.setCursor(60, 25);
  display.println("-");
  display.setTextSize(2);
  display.setCursor(75, 25);
  display.println(now.year(), DEC);
  display.display();
}