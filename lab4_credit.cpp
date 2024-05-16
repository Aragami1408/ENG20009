#include <stdio.h>
#include <stdlib.h>

#include "RTClib.h"

#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>

RTC_DS1307 rtc;

#define TFT_CS 10
#define TFT_RST 6
#define TFT_DC 7

#define TFT_SCLK 13
#define TFT_MOSI 11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.println(F("Setting up Real Time Clock"));
#ifndef ESP8266
  while (!Serial)
    ;
#endif

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  Serial.println(F("Setting up TFT"));

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  Serial.println(F("Initialized"));

  uint16_t time = millis();
  tft.fillScreen(ST77XX_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);


}

void TFTDrawText(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void loop() {
  // put your main code here, to run repeatedly:
  DateTime now = rtc.now();

  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 50);
  tft.setTextColor(0xffff);
  tft.setTextWrap(true);
  tft.setTextSize(3);
  char *time_text = (char *)malloc(10 * sizeof(char));
  snprintf(time_text, 10, "%i:%i:%i", now.hour(), now.minute(), now.second());
  tft.print(time_text);

  //delay(1);
}
