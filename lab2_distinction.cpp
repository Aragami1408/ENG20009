//rows
#define R1 20
#define R2 21
#define R3 2
#define R4 3
#define R5 4
#define R6 5
#define R7 6
#define R8 7

//columns
#define C1 9
#define C2 10
#define C3 11
#define C4 12
#define C5 13
#define C6 14
#define C7 15
#define C8 16

// dip switches
#define B1 22
#define B2 23
#define B3 24
#define B4 25


int zero[8][8] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
};

int one[8][8] = {
  0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0,
};

int two[8][8] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1
};

int three[8][8] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1
};

int four[8][8] = {
  1, 1, 1, 0, 0, 1, 1, 1,
  1, 1, 1, 0, 0, 1, 1, 1,
  1, 1, 1, 0, 0, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 1, 1, 1,
  0, 0, 0, 0, 0, 1, 1, 1,
  0, 0, 0, 0, 0, 1, 1, 1,
};

int five[8][8] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1
};

int six[8][8] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 0, 0, 0, 0, 0, 0,
  1, 1, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
};

int seven[8][8] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 1, 1,
  0, 0, 0, 0, 0, 0, 1, 1,
  0, 0, 0, 0, 0, 0, 1, 1,
  0, 0, 0, 0, 0, 0, 1, 1,
  0, 0, 0, 0, 0, 0, 1, 1,
  0, 0, 0, 0, 0, 0, 1, 1,
};

int eight[8][8] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
};

int nine[8][8] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 1, 1,
  0, 0, 0, 0, 0, 0, 1, 1,
  0, 0, 0, 0, 0, 0, 1, 1,
};

int a[8][8] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
};

int b[8][8] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
};

int c[8][8] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 0, 0, 0, 0, 0, 0,
  1, 1, 0, 0, 0, 0, 0, 0,
  1, 1, 0, 0, 0, 0, 0, 0,
  1, 1, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
};

int d[8][8] = {
  1, 1, 1, 1, 1, 1, 0, 0,
  1, 1, 1, 1, 1, 1, 0, 0,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 0, 0, 0, 0, 1, 1,
  1, 1, 1, 1, 1, 1, 0, 0,
  1, 1, 1, 1, 1, 1, 0, 0,
};

int e[8][8] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
};

int f[8][8] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 0, 0, 0, 0, 0, 0,
  1, 1, 0, 0, 0, 0, 0, 0,
  1, 1, 0, 0, 0, 0, 0, 0,
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //start serial
  //setup output ports
  //set row high and column low to ensure all LEDs are turned off at setup
  //Serial.println("Testing LED Matrix 8x8: ");
  for (int j = 9; j <= 16; j++) {
    pinMode(j, OUTPUT);
    digitalWrite(j, LOW);
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
}

void selectRow(int row) {
  if (row == 1) digitalWrite(R1, LOW); else digitalWrite(R1, HIGH);
  if (row == 2) digitalWrite(R2, LOW); else digitalWrite(R2, HIGH);
  if (row == 3) digitalWrite(R3, LOW); else digitalWrite(R3, HIGH);
  if (row == 4) digitalWrite(R4, LOW); else digitalWrite(R4, HIGH);
  if (row == 5) digitalWrite(R5, LOW); else digitalWrite(R5, HIGH);
  if (row == 6) digitalWrite(R6, LOW); else digitalWrite(R6, HIGH);
  if (row == 7) digitalWrite(R7, LOW); else digitalWrite(R7, HIGH);
  if (row == 8) digitalWrite(R8, LOW); else digitalWrite(R8, HIGH);
}

void resetCol() {
  digitalWrite(C1, LOW);
  digitalWrite(C2, LOW);
  digitalWrite(C3, LOW);
  digitalWrite(C4, LOW);
  digitalWrite(C5, LOW);
  digitalWrite(C6, LOW);
  digitalWrite(C7, LOW);
  digitalWrite(C8, LOW);
}

void selectCol(int column, int state) {
  if (column == 1) digitalWrite(C1, state);
  if (column == 2) digitalWrite(C2, state);
  if (column == 3) digitalWrite(C3, state);
  if (column == 4) digitalWrite(C4, state);
  if (column == 5) digitalWrite(C5, state);
  if (column == 6) digitalWrite(C6, state);
  if (column == 7) digitalWrite(C7, state);
  if (column == 8) digitalWrite(C8, state);
}

int position = 0;

void displayCharacter(int pixels[8][8]) {
  for (int x = 0; x < 5; x++) {
    for (int j = 0; j < 8; j++) {
      resetCol();
      selectRow(j + 1);
      for (int i = 0; i < 8; i++) {
        selectCol(i + 1, pixels[j][abs(i+position)-abs((i+position)/8)*8]);
      }
      resetCol();
      delay(2);
    }
  }
  position += 1;
}

int readDipSwitch() {
  int first = digitalRead(B1);
  int second = digitalRead(B2);
  int third = digitalRead(B3);
  int fourth = digitalRead(B4);

  int value = !first | !second << 1 | !third << 2 | !fourth << 3;
  return value;
}

typedef int pixels[8][8];

void loop() {
  int number = readDipSwitch();
  Serial.print("Number: ");
  Serial.println(number);

  switch (number) {
    case 0:
      displayCharacter(zero);
      break;
    case 1:
      displayCharacter(one);
      break;
    case 2:
      displayCharacter(two);
      break;
    case 3:
      displayCharacter(three);
      break;
    case 4:
      displayCharacter(four);
      break;
    case 5:
      displayCharacter(five);
      break;
    case 6:
      displayCharacter(six);
      break;
    case 7:
      displayCharacter(seven);
      break;
    case 8:
      displayCharacter(eight);
      break;
    case 9:
      displayCharacter(nine);
      break;
    case 10:
      displayCharacter(a);
      break;
    case 11:
      displayCharacter(b);
      break;
    case 12:
      displayCharacter(c);
      break;
    case 13:
      displayCharacter(d);
      break;
    case 14:
      displayCharacter(e);
      break;
    case 15:
      displayCharacter(f);
      break;

  }

}





