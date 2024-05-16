#define R1 20
#define R2 21
#define R3 2
#define R4 3
#define R5 4
#define R6 5
#define R7 6
#define R8 7

#define C1 9
#define C2 10
#define C3 11
#define C4 12
#define C5 13
#define C6 14
#define C7 15
#define C8 16

int button1 = 22;
int button2 = 23;
int button3 = 24;
int button4 = 25;

int buzzer = 8;
bool buzzerOn = false;

int button1State;
int button1LastState;

int button2State;
int button2LastState;

int button3State;
int button3LastState;

int button4State;
int button4LastState;

int pitch = 100;



void setup() {
  Serial.begin(9600);
  for (int j = 9; j <= 16; j++) {
    pinMode(j, OUTPUT);
    digitalWrite(j, HIGH);
  }

  for (int i = 2; i <= 7; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  pinMode(20, OUTPUT);
  digitalWrite(20, HIGH);
  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);

  for (int i = 22; i <= 25; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  pinMode(buzzer, OUTPUT);

}

void activateRow(int row, int state) {
  if (row == 1) digitalWrite(R1, state);
  if (row == 2) digitalWrite(R2, state);
  if (row == 3) digitalWrite(R3, state);
  if (row == 4) digitalWrite(R4, state);
  if (row == 5) digitalWrite(R5, state);
  if (row == 6) digitalWrite(R6, state);
  if (row == 7) digitalWrite(R7, state);
  if (row == 8) digitalWrite(R8, state);
}

void displayPitch(int pitch) {

  switch (pitch) {
    case 1 ... 31:
      digitalWrite(R8, LOW);

      for (int i = 1; i <= 7; i++) {
        activateRow(i, HIGH);
      }

    break;

    case 32 ... 63:
      for (int i = 7; i <= 8; i++) {
        activateRow(i, LOW);
      }

      for (int i = 1; i <= 6; i++) {
        activateRow(i, HIGH);
      }

      break;

    case 64 ... 95:
      for (int i = 6; i <= 8; i++) {
        activateRow(i, LOW);
      }

      for (int i = 1; i <= 5; i++) {
        activateRow(i, HIGH);
      }

      break;

    case 96 ... 126:
      for (int i = 5; i <= 8; i++) {
        activateRow(i, LOW);
      }

      for (int i = 1; i <= 4; i++) {
        activateRow(i, HIGH);
      }

      break;
    
    case 127 ... 158:
      for (int i = 4; i <= 8; i++) {
        activateRow(i, LOW);
      }

      for (int i = 1; i <= 3; i++) {
        activateRow(i, HIGH);
      }

      break;

    case 159 ... 190:
      for (int i = 3; i <= 8; i++) {
        activateRow(i, LOW);
      }

      for (int i = 1; i <= 2; i++) {
        activateRow(i, HIGH);
      }

      break;

    case 191 ... 222:
      for (int i = 2; i <= 8; i++) {
        activateRow(i, LOW);
      }

      digitalWrite(R1, HIGH);

      break;

    case 223 ... 255:
      for (int i = 1; i <= 8; i++) {
        activateRow(i, LOW);
      }

      break;

    default:
      for (int i = 1; i <= 8; i++) {
        activateRow(i, HIGH);
      }
    
  }
}

void loop() {
  byte button1State = digitalRead(button1);

  if (button1State != button1LastState) {
    button1LastState = button1State;
    if (button1State == LOW && !buzzerOn) {
      analogWrite(buzzer, pitch);
      buzzerOn = true;
    }
  }
  
  byte button2State = digitalRead(button2);

  if (button2State != button2LastState) {
    button2LastState = button2State;
    if (button2State == LOW && buzzerOn) {
      noTone(buzzer);
      buzzerOn = false;
    }
  }

  byte button3State = digitalRead(button3);
  if (button3State != button3LastState) {
    button3LastState = button3State;
    if (button3State == LOW && buzzerOn && pitch < 255) {
      pitch += 10;
      if (pitch > 255) {
        pitch = 255;
      }
      analogWrite(buzzer, pitch);
    }
  }

  byte button4State = digitalRead(button4);
  if (button4State != button4LastState) {
    button4LastState = button4State;
    if (button4State == LOW && buzzerOn && pitch > 0) {
      pitch -= 10;
      if (pitch < 1) {
        pitch = 1;
      }
      analogWrite(buzzer, pitch);
    }
  }

  if (buzzerOn) {
    displayPitch(pitch);
  } else {
    for (int i = 1; i <= 8; i++) {
      activateRow(i, HIGH);
    }
  }

  delay(10);
}





