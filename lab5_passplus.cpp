#include <avr/pgmspace.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CS 10
#define TFT_RST 6
#define TFT_DC 7

#define TFT_SCLK 13
#define TFT_MOSI 11

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

const char studentID[] PROGMEM = "103825154";
const char studentName[] PROGMEM = "Cao Minh Vu";
const char unitName[] PROGMEM = "ENG20009 Engineering Technology Inquiry Project";
const char semester[] PROGMEM = "Semester 2, 2023";

const char *const string_table[] PROGMEM = { studentID, studentName, unitName, semester };

char buffer[256];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("OK");

  Serial.println("Setting up TFT");

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  Serial.println("TFT Initialized");

  uint16_t time = millis();
  tft.fillScreen(ST77XX_BLACK);
  time = millis() - time;
}

void scrolltext(int x, int y, const char *s, uint8_t dw = 1, int sz = 1) {
  int16_t x1, y1, wid = tft.width(), inview = 1;
  uint16_t w, h;
  tft.setFont(NULL);
  tft.setTextColor(YELLOW, BLACK);
  tft.setTextSize(sz);
  tft.setTextWrap(false);
  tft.getTextBounds((char *)s, x, y, &x1, &y1, &w, &h);

  for (int steps = wid + w; steps >= 0; steps -= dw) {
    x = steps - w;
    inview = wid - x;
    if (inview > wid) inview = wid;
    if (inview > w) inview = w;
    tft.fillRect(x > 0 ? x : 0, y1, inview + dw, h, BLACK);
    x -= dw;
    tft.setCursor(x, y);
    tft.print(s);
    tft.print("  ");  //rubout trailing chars
    delay(5);
  }
}

void loop() {

  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setFont(NULL);  //System Font
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  for (int i = 0; i < 4; i++) {
    strcpy_P(buffer, (char *)pgm_read_ptr(&(string_table[i])));
    scrolltext(0, 60, buffer, 2, 1);
  }
  delay(2000);
}
