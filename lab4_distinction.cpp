/*!
 * @file readAllData.ino
 * @brief Through the example, you can get the sensor data by using getSensorData:
 * @n     get all data of magnetometer, gyroscope, accelerometer.
 * @n     With the rotation of the sensor, data changes are visible.
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [luoyufeng] (yufeng.luo@dfrobot.com)
 * @maintainer [Fary](feng.yang@dfrobot.com)
 * @version  V1.0
 * @date  2021-10-20
 * @url https://github.com/DFRobot/DFRobot_BMX160
 */

//Use Male/Female jumper wire to connect pin 1 of J9 to pin 21, and pin 3 to pin 20 on Arduino DUE
//Arduino Zero, add shunt as normal to J9

#include <DFRobot_BMX160.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>



#define TFT_CS 10
#define TFT_RST 6
#define TFT_DC 7

#define TFT_SCLK 13
#define TFT_MOSI 11

DFRobot_BMX160 bmx160;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() {
	Serial.begin(9600);
	delay(100);

	//init the hardware bmx160
	if (bmx160.begin() != true) {
		Serial.println("init false");
		while (1)
			;
	}
	//bmx160.setLowPower();   //disable the gyroscope and accelerometer sensor
	bmx160.softReset();  //reset the sensor
	bmx160.wakeUp();     //enable the gyroscope and accelerometer sensor
	/** 
	 * enum{eGyroRange_2000DPS,
	 *       eGyroRange_1000DPS,
	 *       eGyroRange_500DPS,
	 *       eGyroRange_250DPS,
	 *       eGyroRange_125DPS
	 *       }eGyroRange_t;
	 **/
	//bmx160.setGyroRange(eGyroRange_500DPS);

	/**
	 *  enum{eAccelRange_2G,
	 *       eAccelRange_4G,
	 *       eAccelRange_8G,
	 *       eAccelRange_16G
	 *       }eAccelRange_t;
	 */
	// bmx160.setAccelRange(eAccelRange_16G);
	delay(100);

	tft.initR(INITR_BLACKTAB);
	tft.setRotation(3);
	Serial.println(F("Initialized"));

	uint16_t time = millis();
	tft.fillScreen(ST77XX_BLACK);
	time = millis() - time;
}

void loop() {
	sBmx160SensorData_t Omagn, Ogyro, Oaccel;

	/* Get a new sensor event */
	bmx160.getAllData(&Omagn, &Ogyro, &Oaccel);

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

	if (Oaccel.x > 0)
		tft.fillTriangle(0, 64, 30, 88, 30, 40, ST77XX_WHITE);

	if (Oaccel.x < 0)
		tft.fillTriangle(160, 64, 130, 88, 130, 40, ST77XX_WHITE);

	if (Oaccel.y < 0)
		tft.fillTriangle(80, 0, 56, 30, 104, 30, ST77XX_WHITE);

	if (Oaccel.y > 0)
		tft.fillTriangle(80, 128, 56, 98, 104, 98, ST77XX_WHITE);

	delay(200);
	tft.fillScreen(ST77XX_BLACK);
}
