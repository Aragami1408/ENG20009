#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS 10
#define TFT_RST 6
#define TFT_DC 7

#define TFT_SCLK 13
#define TFT_MOSI 11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
RTC_DS1307 rtc;

const int PB1 = 2;

bool analogMode = true;
const int originX = (tft.width() / 2) + 10;
const int originY = (tft.height() / 2) - 15;

#define RAD(degree) ((degree * 71) / 4068)

void toggleMode() {
  analogMode = !analogMode;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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
  Serial.println("RTC Initialized");

  Serial.println(F("Setting up TFT"));

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  Serial.println(F("TFT Initialized"));

  tft.fillScreen(ST77XX_BLACK);

  pinMode(PB1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PB1), toggleMode, RISING);

  // Timer Interrupt Setup
  PORT->Group[PORTA].DIRSET.reg = PORT_PA06;  // Set-up digital pin D8 as an output

  // Set up the generic clock (GCLK4) used to clock timers
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(4) |  // Divide the 48MHz clock source by divisor 1: 48MHz/1=48MHz
                     GCLK_GENDIV_ID(4);    // Select Generic Clock (GCLK) 4
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |          // Set the duty cycle to 50/50 HIGH/LOW
                      GCLK_GENCTRL_GENEN |        // Enable GCLK4
                      GCLK_GENCTRL_SRC_DFLL48M |  // Set the 48MHz clock source
                      GCLK_GENCTRL_ID(4);         // Select GCLK4
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  // Feed GCLK4 to TC4 and TC5
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |      // Enable GCLK4 to TC4 and TC5
                      GCLK_CLKCTRL_GEN_GCLK4 |  // Select GCLK4
                      GCLK_CLKCTRL_ID_TC4_TC5;  // Feed the GCLK4 to TC4 and TC5
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  //TC4->COUNT16.CC[0].reg = 0x2DC5;  // Set the TC4 CC0 register as the TOP value in match frequency mode
  TC4->COUNT16.CC[0].reg = 0x5B8C;
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  //NVIC_DisableIRQ(TC4_IRQn);
  //NVIC_ClearPendingIRQ(TC4_IRQn);
  NVIC_SetPriority(TC4_IRQn, 0);  // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest)
  NVIC_EnableIRQ(TC4_IRQn);       // Connect TC4 to Nested Vector Interrupt Controller (NVIC)

  TC4->COUNT16.INTFLAG.bit.OVF = 1;   // Clear the interrupt flags
  TC4->COUNT16.INTENSET.bit.OVF = 1;  // Enable TC4 interrupts

  TC4->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024 |  // Set prescaler to 1024, 48MHz/1024 = 46.875kHz
                            TC_CTRLA_WAVEGEN_MFRQ |       // Put the timer TC4 into match frequency (MFRQ) mode
                            TC_CTRLA_ENABLE;              // Enable TC4
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY)
    ;
}

void displayDigitalClock() {
  DateTime now = rtc.now();
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(5, tft.height() / 2);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);

  tft.print(now.hour(), DEC);
  tft.print(':');
  if (now.minute() < 10) {
    tft.print("0");
  }
  tft.print(now.minute(), DEC);
  tft.print(':');
  if (now.second() < 10) {
    tft.print("0");
  }
  tft.print(now.second(), DEC);
}

void drawHand(float handAngle, int handRadius) {
  int xHand = originX + handRadius * sin(RAD(handAngle));
  int yHand = originY - handRadius * cos(RAD(handAngle));
  tft.drawLine(originX, originY, xHand, yHand, ST7735_WHITE);
}

void displayAnalogClock() {
  DateTime now = rtc.now();
  tft.fillScreen(ST7735_BLACK);

  float hourAngle = map(now.hour(), 0, 24, 0, 720);
  float minuteAngle = map(now.minute(), 0, 60, 0, 360);
  float secondAngle = map(now.second(), 0, 60, 0, 360);

  drawHand(hourAngle, 20);
  drawHand(minuteAngle, 25);
  drawHand(secondAngle, 30);
}

void TC4_Handler() {
  if (TC4->COUNT16.INTFLAG.bit.OVF && TC4->COUNT16.INTENSET.bit.OVF) {
    if (analogMode)
      displayAnalogClock();
    else
      displayDigitalClock();

    TC4->COUNT16.INTFLAG.bit.OVF = 1;  // Clear the OVF interrupt flag
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
