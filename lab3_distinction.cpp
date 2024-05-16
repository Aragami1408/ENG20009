#include "pitches.h"

const int buzzer = 6; //buzzer to arduino pin 6
const int ldr = A1;
const int pot = A0;
const int pb1 = 2;
const int pb2 = 3;

int duration_offset = 0;
int tune_offset = 0;
bool isTune2Selected = false;

int tune1[] = {NOTE_A4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_E4};
int tune2[] = {NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4};
int duration_arr[] = {300, 300, 300, 150, 600};

void setup(){
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 6 as an output
  pinMode(ldr, INPUT);
  pinMode(pot, INPUT);
  pinMode(pb1, INPUT_PULLUP);
  pinMode(pb2, INPUT_PULLUP);


}

void loop() {
  if (digitalRead(pb1) == LOW)
    isTune2Selected = false;
  if (digitalRead(pb2) == LOW)
    isTune2Selected = true;

  int pot_value = analogRead(pot);
  duration_offset = map(pot_value, 0, 1023, 0, 100);
  //Serial.print("duration_offset: ");
  //Serial.println(duration_offset);

  int ldr_value = analogRead(ldr);
  if (ldr_value <= 100) {
    tune_offset = 261;
  }
  else {
    tune_offset = 0;
  }
  ToneTune();
}

void ToneTune() {

  for (int i = 0; i < 5; i++) {
    int note = isTune2Selected ? tune2[i] : tune1[i];
    note = note + tune_offset;
    int duration = duration_arr[i] - duration_offset;
    tone(buzzer, note, duration);
    delay(duration);
  }
  noTone(buzzer);
  delay(600);
}
