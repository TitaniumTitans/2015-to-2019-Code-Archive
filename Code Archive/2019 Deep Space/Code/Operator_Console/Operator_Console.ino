#include <Joystick.h>
#define ELEVATOR_1 1
#define ELEVATOR_2 2
#define ELEVATOR_3 3
#define INTAKE_CARGO_BAY 4
#define INTAKE_GROUND 5
#define INTAKE_FEEDER 6
#define OVERRIDE 7
#define CLOCK 11
#define LATCH 12
#define DATA 13
#define ELEVATOR_VELOCITY A0
#define HOPPER_VELOCITY A1
#define MODE A2
Joystick_ Joystick(0x3, 0x4, 7, 0, true, true, true, true, false, false, false, false, false, false, false);
void setup() {
	Serial.begin(9600);
	pinMode(ELEVATOR_1, INPUT_PULLUP);
	pinMode(ELEVATOR_2, INPUT_PULLUP);
	pinMode(ELEVATOR_3, INPUT_PULLUP);
	pinMode(INTAKE_CARGO_BAY, INPUT_PULLUP);
	pinMode(INTAKE_GROUND, INPUT_PULLUP);
	pinMode(INTAKE_FEEDER, INPUT_PULLUP);
	pinMode(OVERRIDE, INPUT_PULLUP);
	pinMode(CLOCK, OUTPUT);
	pinMode(LATCH, OUTPUT);
	pinMode(DATA, OUTPUT);
	pinMode(ELEVATOR_VELOCITY, INPUT);
	pinMode(HOPPER_VELOCITY, INPUT);
	pinMode(MODE, INPUT);
	digitalWrite(CLOCK, LOW);
	digitalWrite(LATCH, LOW);
	digitalWrite(DATA, LOW);
	shiftOut(DATA, CLOCK, MSBFIRST, 0x1);
	digitalWrite(DATA, LOW);
	delay(50);
	digitalWrite(LATCH, LOW);
	Joystick.begin();
}
int buttons[7] = {0, 0, 0, 0, 0, 0, 0};
int axes[3] = {0, 0, 0};
int currentValue;
int getInc (int range, int bands) {
	return range / (bands + 1) + 1;
}
boolean inDeadband (int val, int dbs, int bands, int range) {
	int inc = getInc(range, bands);
	for (int i = 1; i <= bands; i++) {
		if (val > inc * i - dbs && val < inc * i + dbs) return true;
	}
	return false;
}
void loop() {
	for (int i = 0; i < 7; i++) {
		currentValue = !digitalRead(i + 1);
		if (buttons[i] != currentValue) {
			buttons[i] = currentValue;
			Joystick.setButton(i, currentValue);
		}
	}
	currentValue = map(analogRead(A0), 0, 1023, 1023, 0);
	if (!inDeadband(currentValue, 50, 2, 1023)) {
		currentValue = floor(currentValue / getInc(1023, 2));
		currentValue = map(currentValue, 0, 2, 0, 1023);
		if (axes[0] != currentValue) {
			axes[0] = currentValue;
			Joystick.setXAxis(currentValue);
		}
	}
	currentValue = map(analogRead(A1), 0, 1023, 1023, 0);
	if (axes[1] != currentValue) {
		axes[1] = currentValue;
		Joystick.setYAxis(currentValue);	
	}
	if (currentValue > 512 - 100 && currentValue < 512 + 100 && axes[1] != 512) {
		currentValue = 512;
		axes[1] = 512;
		Joystick.setYAxis(512);
	}
	currentValue = map(analogRead(A2), 0, 1023, 1023, 0);
	if (axes[2] != currentValue) {
		axes[2] = currentValue;
		Joystick.setZAxis(currentValue);	
	}
	if (currentValue > 512 - 100 && currentValue < 512 + 100 && axes[2] != 512) {
		axes[2] = 512;
		Joystick.setZAxis(512);
	}
	delay(50);
}
