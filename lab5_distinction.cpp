#include <stdio.h>
#include <stdlib.h>

#include <RTClib.h>

#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>

#include <SdFat.h>

RTC_DS1307 rtc;

#define TFT_CS 10
#define TFT_RST 6
#define TFT_DC 7

#define TFT_SCLK 13
#define TFT_MOSI 11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

const int PB1 = 2;
const int PB2 = 3;
const int PB3 = 4;
const int PB4 = 5;

SdFs sd;
FsFile file;

// Chip select may be constant or RAM variable.
const uint8_t SD_CS_PIN = A3;
//
// Pin numbers in templates must be constants.
const uint8_t SOFT_MISO_PIN = 12;
const uint8_t SOFT_MOSI_PIN = 11;
const uint8_t SOFT_SCK_PIN  = 13;

// SdFat software SPI template
SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi;

#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SD_SCK_MHZ(0), &softSpi)

typedef struct {
	int hour;
	int minute;
	int second;
} Clock;

Clock alarm;


void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	Serial.println(F("Initializing RTC"));
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

	Serial.println(F("Finished RTC"));

	Serial.println(F("Initializing TFT"));

	tft.initR(INITR_BLACKTAB);
	tft.setRotation(3);
	tft.fillScreen(ST77XX_BLACK);
	tft.setTextColor(0xffff);
	tft.setTextWrap(true);
	tft.setTextSize(3);

	Serial.println(F("Finished TFT"));

	delay(500);

	pinMode(PB1, INPUT_PULLUP);
	pinMode(PB2, INPUT_PULLUP);
	pinMode(PB3, INPUT_PULLUP);
	pinMode(PB4, INPUT_PULLUP);

	alarm.hour = 0;
	alarm.minute = 0;
	alarm.second = 0;

	if (!sd.begin(SD_CONFIG)) {
		Serial.println("SD card initialization failed!");
		sd.initErrorHalt();
		while (1);
	}

	// Open/create a file for writing
	if (!file.open("message.txt", O_RDWR | O_CREAT)) {
		sd.errorHalt(F("open failed"));
	}

	file.close(); //release file
}

bool alarmSet = false;
bool alarmTriggered = false;
bool is24HourFormat = true;

int pb1PrevState = HIGH;
int pb2PrevState = HIGH;
int pb3PrevState = HIGH;
int pb4PrevState = HIGH;

const int DELAY_TIME = 1000;

void openMenu();
void checkAlarm();
void displayTime();
void setRTCTime(const Clock &clock);
void openSetTimeMenu(String title, Clock &clock);
String clockInformation();
void writeSD(File file, String message);

void loop() {
	// put your main code here, to run repeatedly:
	int pb1State = digitalRead(PB1);
	int pb2State = digitalRead(PB2);

	if (!pb1State && pb1State != pb1PrevState) {
		writeSD(file, clockInformation());
		openMenu();
	}
	pb1PrevState = pb1State;

	if (!pb2State) {
		alarmSet = false;
		writeSD(file, clockInformation());
	}

	checkAlarm();
	displayTime();

	delay(DELAY_TIME);
}

void openMenu() {
	bool running = true;

	while (running) {
		int pb1State = digitalRead(PB1);
		int pb2State = digitalRead(PB2);
		int pb3State = digitalRead(PB3);
		int pb4State = digitalRead(PB4);

		tft.fillScreen(ST77XX_BLACK);
		tft.setTextSize(1);
		tft.setTextColor(ST7735_WHITE);
		tft.setCursor(0, 2);
		tft.setTextWrap(true);
		tft.print("\t PB1. toggle clock format\n");
		if (is24HourFormat) {
			tft.print("\t\t current format: 24h\n");
		}
		else {
			tft.print("\t\t current format: 12h\n");
		}

		tft.print("\t PB2. set time\n");
		tft.print("\t PB3. set alarm\n");
		tft.print("\t PB4. return to clock\n");

		if (!pb1State && pb1State != pb1PrevState) {
			is24HourFormat = !is24HourFormat;
			writeSD(file, clockInformation());
		}
		pb1PrevState = pb1State;

		if (!pb2State && pb2State != pb2PrevState) {
			Clock newTime = {0};
			openSetTimeMenu("Set Time", newTime);
			setRTCTime(newTime);
			writeSD(file, clockInformation());
		}
		pb2PrevState = pb2State;

		if (!pb3State && pb3State != pb3PrevState) {
			openSetTimeMenu("Set Alarm", alarm);
			alarmSet = true;
			writeSD(file, clockInformation());
		}
		pb3PrevState = pb3State;

		if (!pb4State && pb4State != pb4PrevState) {
			running = false;
			writeSD(file, clockInformation());
		}
		pb4PrevState = pb4State;

		delay(DELAY_TIME);
	}
}

void setRTCTime(const Clock &clock) {
	DateTime newTime = DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), clock.hour, clock.minute, clock.second);
	rtc.adjust(newTime);
}

void openSetTimeMenu(String title, Clock &clock) {
	uint8_t hour = 0;
	uint8_t minute = 0;
	uint8_t second = 0;


	bool running = true;

	while (running) {
		int pb1State = digitalRead(PB1);
		int pb2State = digitalRead(PB2);
		int pb3State = digitalRead(PB3);
		int pb4State = digitalRead(PB4);

		tft.fillScreen(ST7735_BLACK);

		tft.setCursor(0, 2);
		tft.setTextSize(2);
		tft.setTextColor(ST7735_WHITE);
		tft.println(title);

		tft.setCursor(0, 20);
		tft.setTextSize(1);
		tft.setTextColor(ST7735_WHITE);
		tft.println("PB1 - Change Hour");
		tft.println("PB2 - Change Minute");
		tft.println("PB3 - Change Second");
		tft.println("PB4 - Save & Quit");

		if (!pb1State) {
			hour++;
			if (hour > 23) hour = 0;
		}

		if (!pb2State) {
			minute++;
			if (minute > 59) minute = 0;
		}

		if (!pb3State) {
			second++;
			if (second > 59) second = 0;
		}

		if (!pb4State) {
			clock.hour = hour;
			clock.minute = minute;
			clock.second = second;
			running = false;
		}
		// Update display
		tft.setCursor(0, 60);
		tft.setTextSize(2);
		tft.setTextColor(ST7735_CYAN);
		char *time_text = (char *)malloc(10 * sizeof(char)); 
		snprintf(time_text, 10, "%02d:%02d:%02d", hour, minute, second);

		tft.print(time_text);

		delay(DELAY_TIME/2);
	}
}

void checkAlarm() {
	if (alarmSet) {
		if (rtc.now().hour() >= alarm.hour && rtc.now().minute() >= alarm.minute && rtc.now().second() >= alarm.second) {
			alarmTriggered = true;
		}
	}
}

void displayTime() {
	DateTime now = rtc.now();

	tft.fillScreen(ST77XX_BLACK);
	tft.setTextSize(2);
	tft.setTextColor(0xffff);
	tft.setCursor(0, 50);

	int hour;
	int minute;
	int second;

	if (is24HourFormat)
		hour = now.hour();
	else {
		if (now.hour() > 12)
			hour = now.hour() - 12;
		else
			hour = now.hour();
	}

	minute = now.minute();
	second = now.second();

	char *time_text = (char *)malloc(10 * sizeof(char));
	snprintf(time_text, 10, "%02d:%02d:%02d", hour, minute, second);
	tft.print(time_text);
	if (!is24HourFormat) {
		tft.print((now.hour() < 12) ? " AM" : " PM");
	}

	if (alarmSet && alarmTriggered) {
		// TODO: blink the tft display or turn the buzzer on when the alarm is triggered
		tft.setTextColor(ST7735_RED);
		tft.setTextSize(2);
		tft.setCursor(tft.width() / 4, 10);
		tft.print("ALARM!");
	}
}
String clockInformation() {
  char information[120];
  DateTime now = rtc.now();
  sprintf(information, "---------------------\nTime: %02d:%02d:%02d\nAlarm: %02d:%02d:%02d\nAlarm active? (0 = no, 1 = yes): %d\n24h format? (0 = no, 1 = yes): %d", now.hour(), now.minute(), now.second(), alarm.hour, alarm.minute, alarm.second, alarmSet, is24HourFormat);
  return information;
}

void writeSD(File file, String message) {
	file.open("message.txt", O_RDWR);
	//file.rewind();
	file.println(message);
	file.close();
}
