const int ledToggle = 11;
const int ledBlink = 10;
const int ledPWM = 9;

int ledToggleState = LOW;
int ledBlinkState = HIGH;
int ledPWMState = LOW;

int ledBlinkInterval = 500;
unsigned long prevMillis = 0;

int ledPWMBrightness = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledToggle, OUTPUT);
  pinMode(ledBlink, OUTPUT);
  pinMode(ledPWM, OUTPUT);
}

void processLEDs() {
  int currentMillis = millis();
  if (currentMillis - prevMillis >= ledBlinkInterval) {
    prevMillis = currentMillis;
    ledBlinkState = !ledBlinkState;
    digitalWrite(ledBlink, ledBlinkState);
  }

  digitalWrite(ledToggle, ledToggleState);
  analogWrite(ledPWM, ledPWMBrightness);
}

void LEDToggleMenu() {
  Serial.println("LED Toggle Menu (Please select from 1-2)");
  Serial.println("1. Toggle on/off");
  Serial.println("2. Back");
  Serial.println("Enter your choice: ");

  while (Serial.available() == 0) { processLEDs(); }

  int selection = Serial.parseInt();

  switch (selection) {
    case 1:
      Serial.println("Bruh you sure to toggle?");
      if (confirmationMenu()) {
        ledToggleState = !ledToggleState;
        Serial.print("Current toggle led state is: ");
        Serial.println(ledToggleState);
      }
      LEDToggleMenu();
      break;
    case 2:
      break;
    default:
      Serial.println("Invalid bruh. Please enter 1-2");
      LEDToggleMenu();
  }
}

void LEDIncreaseMenu() {
  Serial.println("LED Increase Menu (Please select from 1-2)");
  Serial.println("1. Increase Blink Speed");
  Serial.println("2. Back");
  Serial.println("Enter your choice: ");

  while (Serial.available() == 0) { processLEDs(); }

  int selection = Serial.parseInt();

  switch (selection) {
    case 1:
      Serial.println("Bruh you sure to increase?");
      if (confirmationMenu() && ledBlinkInterval > 0) {
        ledBlinkInterval -= 50;
        Serial.print("Current Blink Interval is: ");
        Serial.println(ledBlinkInterval);
      }
      LEDIncreaseMenu();
      break;
    case 2:
      break;
    default:
      Serial.println("Invalid bruh. Please enter 1-2");
      LEDIncreaseMenu();
      break;
  }
}

void LEDDecreaseMenu() {
  Serial.println("LED Decrease Menu (Please select from 1-2)");
  Serial.println("1. Decrease Blink Speed");
  Serial.println("2. Back");
  Serial.println("Enter your choice: ");

  while (Serial.available() == 0) { processLEDs(); }

  int selection = Serial.parseInt();

  switch (selection) {
    case 1:
      Serial.println("Bruh you sure to decrease?");
      if (confirmationMenu() && ledBlinkInterval > 0) {
        ledBlinkInterval += 50;
        Serial.print("Current Blink Interval is: ");
        Serial.println(ledBlinkInterval);
      }
      LEDDecreaseMenu();
      break;
    case 2:
      break;
    default:
      Serial.println("Invalid bruh. Please enter 1-2");
      LEDDecreaseMenu();
      break;
  }
}

void LEDPWMMenu() {
  Serial.println("LED PWM Menu (Please select from 1-2)");
  Serial.println("1. Increase Brightness");
  Serial.println("2. Decrease Brightness");
  Serial.println("3. Back");
  Serial.println("Enter your choice: ");

  while (Serial.available() == 0) { processLEDs(); }

  int selection = Serial.parseInt();

  switch (selection) {
    case 1:
      Serial.println("Bruh you sure to increase brightness?");
      if (confirmationMenu() && ledPWMBrightness < 254) {
        ledPWMBrightness += 50;
        if (ledPWMBrightness > 254) ledPWMBrightness = 254;
      }
      Serial.print("Current PWM LED Brightness: ");
      Serial.println(ledPWMBrightness);
      LEDPWMMenu();
      break;
    case 2:
      Serial.println("Bruh you sure to decrease brightness?");
      if (confirmationMenu() && ledPWMBrightness > 0) {
        ledPWMBrightness -= 50;
        if (ledPWMBrightness < 0) ledPWMBrightness = 0;
      }
      Serial.print("Current PWM LED Brightness: ");
      Serial.println(ledPWMBrightness);
      LEDPWMMenu();
      break;
    case 3:
      break;
    default:
      Serial.println("Invalid bruh. Please enter 1-3");
      LEDPWMMenu();
      break;
  }
}

bool confirmationMenu() {
  Serial.println("1. Yes");
  Serial.println("2. No");
  Serial.println("Enter your choice: ");

  while (Serial.available() == 0) { processLEDs(); }
  int selection = Serial.parseInt();

  switch (selection) {
    case 1:
      return true;
    case 2:
      return false;
    default:
      Serial.println("Invalid bruh. Please enter 1-2");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Main Menu (Please select from 1-4)");
  Serial.println("1. Toggle LED");
  Serial.println("2. Increase Blink Speed");
  Serial.println("3. Decrease Blink Speed");
  Serial.println("4. Change PWM Brightness");

  Serial.println("Enter your choice: ");

  while (Serial.available() == 0) { processLEDs(); }

  int selection = Serial.parseInt();

  switch (selection) {
    case 1:
      LEDToggleMenu();
      break;
    case 2:
      LEDIncreaseMenu();
      break;
    case 3:
      LEDDecreaseMenu();
      break;
    case 4:
      LEDPWMMenu();
      break;
    default:
      Serial.println("Invalid bruh. Please enter 1-4");
  }
}
