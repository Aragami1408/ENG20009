#include <OneWire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CS 10
#define TFT_RST 6
#define TFT_DC 7

#define TFT_SCLK 13
#define TFT_MOSI 11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Define the one wire bus and the address of the DS28E07 chip
OneWire ds(8);  // One wire bus is on digital pin 2
byte addr[] = {0x2D, 0x79 , 0xBC , 0x67 , 0x40 , 0x0 , 0x0 , 0x90}; //insert the Address of EEPROM value into here after you run the searchFunction

String studentID1;
String studentID2;

void searchFunction();
void writeToEEPROM(String data);
String readFromEEPROM(int length);

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  Serial.println("-------------------------------------------------------------------------------");

  Serial.println("Getting Family code / Address of EEPROM:");
  searchFunction();

  Serial.println("Writing studentID1 to EEPROM"); 
  writeToEEPROM("1038251");

  Serial.println("Reading studentID1 from EEPROM"); 
  studentID1 = readFromEEPROM(7);
  studentID1.remove(7, 12);
  Serial.println(studentID1);

  Serial.println("Writing studentID2 to EEPROM"); 
  writeToEEPROM("54");

  Serial.println("Reading studentID2 from EEPROM"); 
  studentID2 = readFromEEPROM(2);
  studentID2.remove(2, 3);
  Serial.println(studentID2);

  // Serial.println("Reading from EEPROM (FROM MEMORY)");       //Reading from eeprom after power-cycle
  // String EEPROMString = readFromEEPROM(8);
  // EEPROMString.remove(7,12);            //trim undefined characters
  // Serial.println(EEPROMString);

  // Read the string back from the EEPROM and print it to the serial monitor
  // Serial.println("Reading from EEPROM");
  // String readString = readFromEEPROM(studentID.length());
  // readString.remove(7,12);
  // Serial.println(readString);

  Serial.println("Setting up TFT");

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  Serial.println("TFT Initialized");

  uint16_t time = millis();
  tft.fillScreen(ST77XX_BLACK);
  time = millis() - time;

  tft.setCursor(0, 5);
  tft.setTextColor(0xffff);
  tft.print(studentID1 + studentID2);
}

void loop() {

}

void searchFunction() {
  byte i;
  byte addr[8];

  if (!ds.search(addr)) {
    ds.reset_search();
    delay(1000);
  }

  for (i = 0; i < 8; i++) {
    Serial.print(addr[i], HEX);
    Serial.print(" ");
  }
  Serial.print("\n");
}

// Function to write a string to the DS28E07 EEPROM
void writeToEEPROM(String data) {
  // Convert the input string to a char array
  char dataChars[30];
  data.toCharArray(dataChars, 30);

  // Write the data to the EEPROM
  ds.reset(); //reset device
  ds.select(addr);//select address of device to talk to
  ds.write(0x0F,1);   // Write ScratchPad       Configuration/mode bytes can be in datasheet
  ds.write(0x01,1);   //TA1 addresses           TA (Target Address) can also be found in the datasheet
  ds.write(0x09,1);   //TA2 addresses
  for ( int i = 0; i < data.length(); i++) {
    ds.write(dataChars[i],1);
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x0F, 1);  // Copy ScratchPad
}

// Function to read a string from the DS28E07 EEPROM
String readFromEEPROM(int length) {
  byte dataBytes[30];
  ds.reset();
  ds.select(addr);
  ds.write(0xAA);  // Read Scratchpad
  for (int i = 0; i < length+4; i++) {
    dataBytes[i] = ds.read();
  }
  return String((char*)dataBytes).substring(3);
}
