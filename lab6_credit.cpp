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

void timerInterruptSetup() {
	Serial.begin(9600);

	PORT->Group[PORTA].DIRSET.reg = PORT_PA06;       // Set-up digital pin D8 as an output

	// Set up the generic clock (GCLK4) used to clock timers
	GCLK->GENDIV.reg = GCLK_GENDIV_DIV(4) |          // Divide the 48MHz clock source by divisor 1: 48MHz/1=48MHz
		GCLK_GENDIV_ID(4);             // Select Generic Clock (GCLK) 4
	while (GCLK->STATUS.bit.SYNCBUSY);               // Wait for synchronization

	GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
		GCLK_GENCTRL_GENEN |         // Enable GCLK4
		GCLK_GENCTRL_SRC_DFLL48M |   // Set the 48MHz clock source
		GCLK_GENCTRL_ID(4);          // Select GCLK4
	while (GCLK->STATUS.bit.SYNCBUSY);               // Wait for synchronization

	// Feed GCLK4 to TC4 and TC5
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TC4 and TC5
		GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
		GCLK_CLKCTRL_ID_TC4_TC5;     // Feed the GCLK4 to TC4 and TC5
	while (GCLK->STATUS.bit.SYNCBUSY);               // Wait for synchronization

	TC4->COUNT16.CC[0].reg = 0xB71A;                 // Set the TC4 CC0 register as the TOP value in match frequency mode
	while (TC4->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization

	//NVIC_DisableIRQ(TC4_IRQn);
	//NVIC_ClearPendingIRQ(TC4_IRQn);
	NVIC_SetPriority(TC4_IRQn, 0);    // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest)
	NVIC_EnableIRQ(TC4_IRQn);         // Connect TC4 to Nested Vector Interrupt Controller (NVIC)

	TC4->COUNT16.INTFLAG.bit.OVF = 1;                        // Clear the interrupt flags
	TC4->COUNT16.INTENSET.bit.OVF = 1;                       // Enable TC4 interrupts

	TC4->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024 |   // Set prescaler to 1024, 48MHz/1024 = 46.875kHz
		TC_CTRLA_WAVEGEN_MFRQ |        // Put the timer TC4 into match frequency (MFRQ) mode
		TC_CTRLA_ENABLE;               // Enable TC4
	while (TC4->COUNT16.STATUS.bit.SYNCBUSY);
}

void setup() {

	timerInterruptSetup();

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

	bmx160.getAllData(NULL, NULL, &Oaccel);

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
}

void TC4_Handler() {
	/* Get a new sensor event */
	if (TC4->COUNT16.INTFLAG.bit.OVF && TC4->COUNT16.INTENSET.bit.OVF) {
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
		tft.fillScreen(ST77XX_BLACK);

		currentIndex = (currentIndex + 1) % 600;

		TC4->COUNT16.INTFLAG.bit.OVF = 1;
	}
}
