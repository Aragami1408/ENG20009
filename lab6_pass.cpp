#include <Arduino.h>


#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>

#define BUTTON_PIN 2

#define TFT_CS 10
#define TFT_RST 6
#define TFT_DC 7

#define TFT_SCLK 13
#define TFT_MOSI 11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void printToLCD();

void setup() {
	Serial.begin(9600);

	pinMode(BUTTON_PIN, INPUT_PULLUP);
	attachInterrupt(BUTTON_PIN, printToLCD, LOW);

	Serial.println(F("Initializing TFT"));

	tft.initR(INITR_BLACKTAB);
	tft.setRotation(3);
	tft.fillScreen(ST77XX_BLACK);
	tft.setTextColor(0xffff);
	tft.setTextWrap(true);
	tft.setTextSize(3);

	Serial.println(F("Finished TFT"));
}

void printToLCD() {
	tft.setCursor(tft.width() / 2, tft.height() / 2);
	tft.print("@");
}

void loop() {
	
}
