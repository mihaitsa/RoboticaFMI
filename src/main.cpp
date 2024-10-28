#include <Arduino.h>

#define led1 10 // 25%
#define led2 9	// 50%
#define led3 8	// 75%
#define led4 7	// 100%

#define ledR 6 // rosu
#define ledG 5 // verde
#define ledB 4 // albastru

#define buttonStart 3
#define buttonStop 2

bool charging = false;
bool stationFree = true;
unsigned long lastMillis;
int currentStep = 0;

const unsigned long CHARGE_DELAY = 3000; // 3 seconds between steps
const int STOP_PRESS_DURATION = 100;     // 1 second for stop press
const int DEBOUNCE_DELAY = 50;           // 50 ms debounce


void resetStation();
void startCharging();
void stopCharging();
void updateChargingProgress();
void blinkLED(int ledPin);
void blinkAllLEDs();

void setup() {
	pinMode(led1, OUTPUT);
	pinMode(led2, OUTPUT);
	pinMode(led3, OUTPUT);
	pinMode(led4, OUTPUT);

	pinMode(ledR, OUTPUT);
	pinMode(ledG, OUTPUT);
	pinMode(ledB, OUTPUT);

	pinMode(buttonStart, INPUT_PULLUP);
	pinMode(buttonStop, INPUT_PULLUP);

	resetStation();
}

void loop() {

	if (digitalRead(buttonStart) == LOW && stationFree) {
		delay(DEBOUNCE_DELAY); // debouncing
		startCharging();
	}

	if (digitalRead(buttonStop) == LOW && !stationFree) {
		long pressDuration = 0;
		while (digitalRead(buttonStop) == LOW) {
			pressDuration++;
			delay(10);
			if (pressDuration > STOP_PRESS_DURATION) {
				stopCharging();
				break;
			}
		}
	}

	if (charging) {
		updateChargingProgress();
	}
}

void resetStation() {
	stationFree = true;
	charging = false;
	currentStep = 0;

	digitalWrite(led1, LOW);
	digitalWrite(led2, LOW);
	digitalWrite(led3, LOW);
	digitalWrite(led4, LOW);

	digitalWrite(ledR, LOW);
	digitalWrite(ledG, HIGH);
	digitalWrite(ledB, LOW);
}

void startCharging() {
	stationFree = false;
	charging = true;

	digitalWrite(ledR, HIGH);
	digitalWrite(ledG, LOW);
	digitalWrite(ledB, LOW);

	lastMillis = millis();
	currentStep = 1; // we start at 25%
}

void stopCharging() {
	charging = false;
	blinkAllLEDs();
	resetStation();
}

void updateChargingProgress() {
	if (millis() - lastMillis >= CHARGE_DELAY) {
		lastMillis = millis();

		if (currentStep == 1) {
			blinkLED(led1);
		}
		else if (currentStep == 2) {
			digitalWrite(led1, HIGH);
			blinkLED(led2);
		}
		else if (currentStep == 3) {
			digitalWrite(led2, HIGH);
			blinkLED(led3);
		}
		else if (currentStep == 4) {
			digitalWrite(led3, HIGH);
			blinkLED(led4);
		}
		else if (currentStep == 5) {
			digitalWrite(led4, HIGH);
			blinkAllLEDs();
			resetStation();
		}
		currentStep++;
	}
}

void blinkAllLEDs() {
	for (int i = 0; i < 3; i++) {
		digitalWrite(led1, HIGH);
		digitalWrite(led2, HIGH);
		digitalWrite(led3, HIGH);
		digitalWrite(led4, HIGH);
		delay(500);
		digitalWrite(led1, LOW);
		digitalWrite(led2, LOW);
		digitalWrite(led3, LOW);
		digitalWrite(led4, LOW);
		delay(500);
	}
}

void blinkLED(int ledPin) {
	for (int i = 0; i < 3; i++){
		digitalWrite(ledPin, HIGH);
		delay(500);
		digitalWrite(ledPin, LOW);
		delay(500);
	}
	
	digitalWrite(ledPin, HIGH);
}