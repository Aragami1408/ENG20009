// Pin numbers definition for respective LEDs and push buttons
const int ledTop = 11;
const int ledBlink = 10;
const int ledPWM = 9;

const int buttonToggle = 2;
const int buttonSpeedUp = 3;
const int buttonSpeedDown = 4;
const int buttonBrightness = 5;

int lastButtonState = 0;
int currentButtonState = 0;

int blinkSpeed = 500; // Initial blink speed in milliseconds
int brightnessValue = 128; // Initial brightness value (0 to 254)


void setup() {
  // Initialize LEDs as OUTPUT
  pinMode(ledTop, OUTPUT);
  pinMode(ledBlink, OUTPUT);
  pinMode(ledPWM, OUTPUT);

  // Initialize push buttons as INPUT with pull-up resistors
  pinMode(buttonToggle, INPUT_PULLUP);
  pinMode(buttonSpeedUp, INPUT_PULLUP);
  pinMode(buttonSpeedDown, INPUT_PULLUP);
  pinMode(buttonBrightness, INPUT_PULLUP);

  // Initial state
  digitalWrite(ledTop, LOW);

  // Start Serial comm for debugging
  Serial.begin(9600);
}

void loop() {
  // Toggle top LED
  currentButtonState = digitalRead(buttonToggle);
  if (currentButtonState != lastButtonState) {
    if (currentButtonState == LOW) {
      if (digitalRead(ledTop) == HIGH) {
        digitalWrite(ledTop, LOW);
      }
      else {
        digitalWrite(ledTop, HIGH);
      }
    }
    delay(50);
  }
  // Blink second from top LED
  digitalWrite(ledBlink, !digitalRead(ledBlink));
  delay(blinkSpeed);

  // Increase blink speed
  if (digitalRead(buttonSpeedUp) == LOW) {
    if (blinkSpeed <= 50) blinkSpeed = 50;
    else blinkSpeed -= 50;
  }

  // Decrease blink speed
  if (digitalRead(buttonSpeedDown) == LOW) {
    if (blinkSpeed >= 1000) blinkSpeed = 1000;
    else blinkSpeed += 50;
  }

  if (digitalRead(buttonBrightness) == LOW) {
    brightnessValue = min(254, brightnessValue + 10);
    analogWrite(ledPWM, brightnessValue);
  }

  // Print blink speed and brightness value to Serial Monitor
  Serial.print("Blink Speed: ");
  Serial.print(blinkSpeed);
  Serial.print(" ms\t Brightness: ");
  Serial.println(brightnessValue);

  lastButtonState = currentButtonState;
}
