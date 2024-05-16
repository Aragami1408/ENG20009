const int LED = 11;
const int LDR = A1;

void setup() {
  // put your setup code here, to run once:

  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int ldrValue = analogRead(LDR);
  if (ldrValue < 90) {
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }
}
