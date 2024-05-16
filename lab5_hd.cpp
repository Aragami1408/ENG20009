#include <Arduino.h>

#include <DFRobot_BMX160.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <RTClib.h>
#include <SdFat.h>

#define TFT_CS 10
#define TFT_RST 6
#define TFT_DC 7

#define TFT_SCLK 13
#define TFT_MOSI 11

DFRobot_BMX160 bmx160;
sBmx160SensorData_t Oaccel;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

SdFs sd;
FsFile file;

RTC_DS1307 rtc;

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

float xs[600];
float ys[600];
float zs[600];
int currentIndex = 0;

#define FILE_NAME "imuData.txt"

void setup() {
	Serial.begin(9600);
	delay(100);

	Serial.println(F("Initializing IMU"));
	//init the hardware bmx160
	if (bmx160.begin() != true) {
		Serial.println("IMU failed to initialize");
		while (1)
			;
	}
	//bmx160.setLowPower();   //disable the gyroscope and accelerometer sensor
	bmx160.softReset();  //reset the sensor
	bmx160.wakeUp();     //enable the gyroscope and accelerometer sensor
	bmx160.setAccelRange(eAccelRange_4G);
	delay(100);

	bmx160.getAllData(NULL, NULL, &Oaccel);

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

	Serial.println(F("Finished TFT"));

	Serial.println(F("SD card initialization"));
	if (!sd.begin(SD_CONFIG)) {
		Serial.println("SD card initialization failed!");
		sd.initErrorHalt();
		while (1);
	}

	// Open/create a file for writing
	if (!file.open(FILE_NAME, O_RDWR | O_CREAT)) {
		sd.errorHalt(F("open failed"));
	}

	if (sd.exists(FILE_NAME)) {
		if (sd.remove(FILE_NAME)) {
			Serial.println("file deleted");
		}
		else {
			Serial.println("cant delete");
		}
	}
	else {
		Serial.println("File does not exist");
	}

	file.close(); //release file
}

void drawGraphLine(float data[], uint16_t color) {
	int i = currentIndex;
	int xpos = 60;
	if (i > 0) { 
		for (i = currentIndex; i > 0; i--) {
			tft.drawLine(xpos, data[i], xpos + 10, data[i-1], color);
			xpos += 10;
		}
	}
}

String concatSensorData(float x, float y, float z, int hours, int minutes, int seconds) {
	char text[50];
	sprintf(text, "%02d:%02d:%02d - x: %f, y:%f, z:%f", hours, minutes, seconds, x, y, z);
	return text;
}

void writeSD(File file, String message) {
	file.open(FILE_NAME, FILE_WRITE);
	file.println(message);
	file.close();
}

void loop() {
  /* Get a new sensor event */
  bmx160.getAllData(NULL, NULL, &Oaccel);
  DateTime now = rtc.now();

  /* Display the accelerometer results (accelerometer data is in m/s^2) */
  Serial.print("A ");
  Serial.print("X: ");
  Serial.print(Oaccel.x);
  Serial.print("  ");
  Serial.print("Y: ");
  Serial.print(Oaccel.y);
  Serial.print("  ");
  Serial.print("Z: ");
  Serial.print(Oaccel.z);
  Serial.print("  ");
  Serial.println("m/s^2");

  Serial.println("");

	tft.setCursor(5, 45);
	tft.setTextColor(ST77XX_RED);
	tft.println("X (Red):");
	tft.setTextColor(ST77XX_GREEN);
	tft.println("Y (Green):");
	tft.setTextColor(ST77XX_BLUE);
	tft.println("Z (Blue):");

	xs[currentIndex] = map(Oaccel.x, -100, 100, 128, 0);
	ys[currentIndex] = map(Oaccel.y, -100, 100, 128, 0);
	zs[currentIndex] = map(Oaccel.z, -100, 100, 128, 0);

	drawGraphLine(xs, ST77XX_RED);
	drawGraphLine(ys, ST77XX_GREEN);
	drawGraphLine(zs, ST77XX_BLUE);

	String data = concatSensorData(Oaccel.x, Oaccel.y, Oaccel.z, now.hour(), now.minute(), now.second());
	writeSD(file, data);
	delay(1000);
	tft.fillScreen(ST77XX_BLACK);

	currentIndex = (currentIndex + 1) % 600;
}
