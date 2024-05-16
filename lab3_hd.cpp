#include <Arduino.h>

#define C1 A0
#define C2 A1
#define C3 A2
#define C4 A3
#define C5 A4
#define C6 A5
#define C7 20
#define C8 21

#define R1 13
#define R2 12
#define R3 11
#define R4 10
#define R5 9
#define R6 8
#define R7 7
#define R8 6

#define button 2
#define pot A0

int position = 0;
int selection = 0;
int buttonLastState;

int sine[8][16] {
	0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,
		0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,
		0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,
		1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
		1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
		0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,
		0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,
		0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,
};

int triangle[8][16] {
	0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
		0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,
		0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,
		0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,
		0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,
		0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
};

int square[8][16] {
	1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,
		1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
		1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
		1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
		1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
		1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
		1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
		1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1
};

int sawtooth[8][16] {
	0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,
		0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,
		0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,
		0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,
		0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
		0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,1,
		0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,1,
		1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1
};


void setup() {
	Serial.begin(9600);
	pinMode(button, INPUT_PULLUP);
	pinMode(pot, INPUT);

	pinMode(R1, OUTPUT);
	pinMode(R2, OUTPUT);
	pinMode(R3, OUTPUT);
	pinMode(R4, OUTPUT);
	pinMode(R5, OUTPUT);
	pinMode(R6, OUTPUT);
	pinMode(R7, OUTPUT);
	pinMode(R8, OUTPUT);

	pinMode(C1, OUTPUT);
	pinMode(C2, OUTPUT);
	pinMode(C3, OUTPUT);
	pinMode(C4, OUTPUT);
	pinMode(C5, OUTPUT);
	pinMode(C6, OUTPUT);
	pinMode(C7, OUTPUT);
	pinMode(C8, OUTPUT);

	buttonLastState = digitalRead(button);
}

void scanRow(int row) {
	if (row == 1) digitalWrite(R1, LOW); else digitalWrite(R1, HIGH);
	if (row == 2) digitalWrite(R2, LOW); else digitalWrite(R2, HIGH);
	if (row == 3) digitalWrite(R3, LOW); else digitalWrite(R3, HIGH);
	if (row == 4) digitalWrite(R4, LOW); else digitalWrite(R4, HIGH);
	if (row == 5) digitalWrite(R5, LOW); else digitalWrite(R5, HIGH);
	if (row == 6) digitalWrite(R6, LOW); else digitalWrite(R6, HIGH);
	if (row == 7) digitalWrite(R7, LOW); else digitalWrite(R7, HIGH);
	if (row == 8) digitalWrite(R8, LOW); else digitalWrite(R8, HIGH);
}

void activateColumn(int column, int state) {
	if (column == 1) digitalWrite(C1, state);  
	if (column == 2) digitalWrite(C2, state); 
	if (column == 3) digitalWrite(C3, state);  
	if (column == 4) digitalWrite(C4, state); 
	if (column == 5) digitalWrite(C5, state);  
	if (column == 6) digitalWrite(C6, state); 
	if (column == 7) digitalWrite(C7, state);  
	if (column == 8) digitalWrite(C8, state); 
}

void displayWave(int wave[8][16], int frequency) {
	for (int x = 0; x < 5; x++) {
		for (int j = 0; j < 8; j++){
			scanRow(j + 1);
			for (int i = 0; i < 16; i++) {
				int columnPosition = (i - position + 16) % 16;            // Calculates column position for scrolling
				activateColumn(i + 1, wave[j][columnPosition]);
			}
			delay(2);                                                 // Scanning delay
		}
	}

	position = (position + frequency) % 16; // Basically allows the position to reset once it reaches 8

}



void loop() {
	int potValue = analogRead(pot);
	int frequency = map(potValue, 0, 1023, 1, 5);



	byte buttonState = digitalRead(button);

	if (buttonState != buttonLastState) {
		buttonLastState = buttonState;
		if (buttonState == LOW) {
			selection++;
			if (selection == 4) {
				selection = 0;
			}
		}
	}

	switch (selection) {
		case 1:
			displayWave(triangle, frequency); // triangle
			break;
		case 2:
			displayWave(square, frequency);   // square
			break;
		case 3:
			displayWave(sawtooth, frequency); // sawtooth
			break;
		default:
			displayWave(sine, frequency);     // sine
	}

}
