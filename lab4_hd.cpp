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

#include <Arduino.h>

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
sBmx160SensorData_t Oaccel;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

int symX = 76, symY = 55;
int speedX = 0, speedY = 0;
int accelInitX = 0, accelInitY = 0;

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
  bmx160.setAccelRange(eAccelRange_4G);
  delay(100);

  bmx160.getAllData(NULL, NULL, &Oaccel);
  accelInitX = Oaccel.x;
  accelInitY = Oaccel.y;

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  Serial.println(F("Initialized"));

  uint16_t time = millis();
  tft.fillScreen(ST77XX_BLACK);
  time = millis() - time;
}

void loop() {

  /* Get a new sensor event */
  bmx160.getAllData(NULL, NULL, &Oaccel);

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

  speedX = accelInitX + -Oaccel.x;
  speedY = accelInitY + Oaccel.y;

  symX = symX + speedX;
  symY = symY + speedY;

  if (symX < 1) symX = 1;
  if (symX > 151) symX = 151;
  if (symY < -1) symY = -1;
  if (symY > 118) symY = 118;

  tft.setCursor(symX, symY);
  tft.print("@");

  delay(100);
  tft.fillScreen(ST77XX_BLACK);
}
