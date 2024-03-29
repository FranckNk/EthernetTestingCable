/*

TITRE          : Test de 02 Cables RJ45 MKR1000
AUTEUR         : Franck Nkeubou Awougang
DATE           : 28/01/2022
DESCRIPTION    : Programme pour tester deux cables RJ45, ce uC sera considéré comme éméteur du 
				 signal.
				 Inspiration du code BuitinExample de Arduino pour le rebondissement. Source : https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
VERSION        : 0.0.2

*/

// Librairies.
#include <Arduino.h>
#include "Timer.h"
#include "Functions.h"

// Declaration d'une instance de la classe Timer.
Timer Temp;
Timer TempPrint;
RJ45_Female MaTeteFemelle;

// Déclaration des constantes.
// Les LEDs.
const short int PIN_GREEN_LED1 = A1;
const short int PIN_GREEN_LED2 = A2;
const short int PIN_GREEN_LED3 = A3;
const short int PIN_GREEN_LED4 = A4;
const short int PIN_MODE_DROIT = 9;
const short int PIN_MODE_CROISE = 10;

// Les Pin pour le RJ45 Femelle.
const short int PIN_BLUE     	 = 4;
const short int PIN_GREEN        = 6;
const short int PIN_ORANGE  	 = 2;
const short int PIN_MARRON  	 = 8;
const short int PIN_BLUE_WHITE 	 = 5;
const short int PIN_GREEN_WHITE  = 3;
const short int PIN_ORANGE_WHITE = 1;
const short int PIN_MARRON_WHITE = 7;

// Declaration du tableau de toutes les pins du RJ45_Female
short int PinsValues[8] = {PIN_ORANGE_WHITE, PIN_ORANGE, PIN_GREEN_WHITE, PIN_BLUE, 
							PIN_BLUE_WHITE, PIN_GREEN, PIN_MARRON_WHITE, PIN_MARRON};

// Déclaration des variables.
// Variables will change:
short int ledState = LOW;         // the current state of the output pin
short int buttonState;             // the current reading from the input pin
short int lastButtonState = HIGH;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

short int PinSwitchMode   	 = 0;

// Declaration des fonctions.
void ActiveCableEthernet(short int PinBroche);
void DesactiveCableEthernet();
void LEDModeON();

void setup() {

	Serial.begin(9600);

	// Configuration des sorties des broches du RJ45 Femalle.
	MaTeteFemelle.SetPinsRJ45(PinsValues);
	MaTeteFemelle.InitialisationRJ45F();

	// Configuration de la broche de décision.
	pinMode(PinSwitchMode, INPUT);

	pinMode(PIN_MODE_CROISE, OUTPUT);
	pinMode(PIN_MODE_DROIT, OUTPUT);

}

void loop() {
	// put your main code here, to run repeatedly:
	int reading = digitalRead(PinSwitchMode);

	// check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH), and you've waited long enough
	// since the last press to ignore any noise:

	// If the switch changed, due to noise or pressing:
	if (reading != lastButtonState) {
	// reset the debouncing timer
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > debounceDelay) {
		// whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != buttonState) {
			buttonState = reading;

			// only toggle the LED if the new button state is HIGH
			if (buttonState == HIGH) {
				ledState = !ledState;
				MaTeteFemelle.SendSignal();
				LEDModeON(); // On change de mode sur la LED.
			}
		}
	}
	
	// save the reading. Next time through the loop, it'll be the lastButtonState:
	lastButtonState = reading;
}



void LEDModeON(){
	if (ledState){
		analogWrite(PIN_MODE_DROIT, 255);
		analogWrite(PIN_MODE_CROISE, 0);
	}
	else{
		analogWrite(PIN_MODE_DROIT, 0);
		analogWrite(PIN_MODE_CROISE, 255);
	}
}

void DesactiveCableEthernet(){
	analogWrite(PIN_BLUE, 0);
	analogWrite(PIN_GREEN, 0);
	analogWrite(PIN_ORANGE, 0);
	analogWrite(PIN_MARRON, 0);
	analogWrite(PIN_BLUE_WHITE, 0);
	analogWrite(PIN_GREEN_WHITE, 0);
	analogWrite(PIN_ORANGE_WHITE, 0);
	analogWrite(PIN_MARRON_WHITE, 0);
}


