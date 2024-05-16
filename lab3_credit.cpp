const int pot = A0;

const int buzzer = 6;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pot, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int potValue = analogRead(pot) / 4;
  int buzzerVol = potValue / 25;
  Serial.println(buzzerVol);
  analogWrite(buzzer, buzzerVol);
}
