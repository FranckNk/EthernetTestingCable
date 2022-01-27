/*

TITRE          : Test de 02 Cables RJ45
AUTEUR         : Franck Nkeubou Awougang
DATE           : 28/01/2022
DESCRIPTION    : Programme pour tester deux cables RJ45, ce uC sera considéré comme éméteur du 
				 signal.
				 Inspiration du code BuitinExample de Arduino pour le rebondissement. Source : https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
VERSION        : 0.0.1

*/

// Librairies.
#include <Arduino.h>
#include "DFRobot_RGBLCD1602.h"
#include "Timer.h"

// Déclaration de la classe LCD pour notre afficheur.
DFRobot_RGBLCD1602 lcd(/*lcdCols*/16,/*lcdRows*/2);  //16 characters and 2 lines of show 

// Declaration d'une instance de la classe Timer.
Timer Temp;
Timer TempPrint;

// Déclaration des constantes.
// Les LEDs.
const short int PIN_GREEN_LED1 = A1;
const short int PIN_GREEN_LED2 = A2;
const short int PIN_GREEN_LED3 = A3;
const short int PIN_GREEN_LED4 = A4;
// Les Pin pour le RJ45 Femelle.
const short int PIN_BLUE     	 = 4;
const short int PIN_GREEN        = 2;
const short int PIN_ORANGE  	 = 6;
const short int PIN_MARRON  	 = 8;
const short int PIN_BLUE_WHITE 	 = 5;
const short int PIN_GREEN_WHITE  = 1;
const short int PIN_ORANGE_WHITE = 3;
const short int PIN_MARRON_WHITE = 7;

// Déclaration des variables.
// Variables will change:
short int ledState = HIGH;         // the current state of the output pin
short int buttonState;             // the current reading from the input pin
short int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

short int PinSwitchMode   	 = 0;
short int IdLED = 1;
unsigned long TimeDelay 	 = 500;
unsigned long TimeDelayPrint = 2000;
unsigned long TimeDelayTemp  = 0;


void SelectLED(short int NumLED){
	switch (NumLED)
	{
	case 1:
		digitalWrite(PIN_GREEN_LED1, 1);
		digitalWrite(PIN_GREEN_LED2, 0);
		digitalWrite(PIN_GREEN_LED3, 0);
		digitalWrite(PIN_GREEN_LED4, 0);
		break;
	case 2:
		digitalWrite(PIN_GREEN_LED1, 0);
		digitalWrite(PIN_GREEN_LED2, 1);
		digitalWrite(PIN_GREEN_LED3, 0);
		digitalWrite(PIN_GREEN_LED4, 0);
		break;
	case 3:
		digitalWrite(PIN_GREEN_LED1, 0);
		digitalWrite(PIN_GREEN_LED2, 0);
		digitalWrite(PIN_GREEN_LED3, 1);
		digitalWrite(PIN_GREEN_LED4, 0);
		break;
	
	case 4:
		digitalWrite(PIN_GREEN_LED1, 0);
		digitalWrite(PIN_GREEN_LED2, 0);
		digitalWrite(PIN_GREEN_LED3, 0);
		digitalWrite(PIN_GREEN_LED4, 1);
		break;
	default:
		break;
	}
}

void ActiveCableEthernet(){
	digitalWrite(PIN_BLUE, HIGH);
	digitalWrite(PIN_GREEN, HIGH);
	digitalWrite(PIN_ORANGE, HIGH);
	digitalWrite(PIN_MARRON, HIGH);
	digitalWrite(PIN_BLUE_WHITE, HIGH);
	digitalWrite(PIN_GREEN_WHITE, HIGH);
	digitalWrite(PIN_ORANGE_WHITE, HIGH);
	digitalWrite(PIN_MARRON_WHITE, HIGH);
}

void DesactiveCableEthernet(){
	digitalWrite(PIN_BLUE, LOW);
	digitalWrite(PIN_GREEN, LOW);
	digitalWrite(PIN_ORANGE, LOW);
	digitalWrite(PIN_MARRON, LOW);
	digitalWrite(PIN_BLUE_WHITE, LOW);
	digitalWrite(PIN_GREEN_WHITE, LOW);
	digitalWrite(PIN_ORANGE_WHITE, LOW);
	digitalWrite(PIN_MARRON_WHITE, LOW);
}


void setup() {

	Serial.begin(9600);
	//Configuration des sorties des LEDs.
	pinMode(PIN_GREEN_LED1, OUTPUT);
	pinMode(PIN_GREEN_LED2, OUTPUT);
	pinMode(PIN_GREEN_LED3, OUTPUT);
	pinMode(PIN_GREEN_LED4, OUTPUT);

	// Configuration des sorties des broches du RJ45 Femalle.
	pinMode(PIN_BLUE, INPUT);
	pinMode(PIN_GREEN, INPUT);
	pinMode(PIN_ORANGE, INPUT);
	pinMode(PIN_MARRON, INPUT);
	pinMode(PIN_BLUE_WHITE, INPUT);
	pinMode(PIN_GREEN_WHITE, INPUT);
	pinMode(PIN_ORANGE_WHITE, INPUT);
	pinMode(PIN_MARRON_WHITE, INPUT);

	// Configuration de la broche de décision.
	pinMode(PinSwitchMode, INPUT);
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
					IdLED = 1;
					Temp.startTimer(TimeDelay);
				}
			}
		}

	if (ledState){
		// set the LED:
		ActiveCableEthernet();
		SelectLED(IdLED);
		if (Temp.isTimerReady()){
			if (IdLED == 4)
				IdLED = 1;
			else
				IdLED += 1;			
			Temp.startTimer(TimeDelay);
		}
	}
	else{
		DesactiveCableEthernet();
		digitalWrite(PIN_GREEN_LED1, 0);
		digitalWrite(PIN_GREEN_LED2, 0);
		digitalWrite(PIN_GREEN_LED3, 0);
		digitalWrite(PIN_GREEN_LED4, 0);
	}
	

	// save the reading. Next time through the loop, it'll be the lastButtonState:
	lastButtonState = reading;

	// save the reading. Next time through the loop, it'll be the lastButtonState:
	lastButtonState = reading;
}
