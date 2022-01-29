/*

TITRE          : Test de 02 Cables RJ45 MKR1000
AUTEUR         : Franck Nkeubou Awougang
DATE           : 28/01/2022
DESCRIPTION    : Programme pour tester deux cables RJ45, ce uC sera considéré comme éméteur du 
				 signal.
				 Inspiration du code BuitinExample de Arduino pour le rebondissement. Source : https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
VERSION        : 0.0.1

*/

// Librairies.
#include <Arduino.h>
#include "Timer.h"
#include "Functions.h"

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

// Déclaration des LED des couleurs identificatives.
LedFromColor GreenLEDs[4];

// Déclaration des variables.
// Variables will change:
short int ledState = LOW;         // the current state of the output pin
short int buttonState;             // the current reading from the input pin
short int lastButtonState = HIGH;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

short int PinSwitchMode   	 = 0;
short int IdLED = 0;
unsigned long TimeDelayLED 	 = 2000;
unsigned long TimeDelayPrint = 2000;
unsigned long TimeDelayTemp  = 0;

// Declaration des fonctions.
void SelectLED(short int NumLED);
void ActiveCableEthernet(short int PinBroche);
void DesactiveCableEthernet();
void SendSignal(short int ExceptPin);

void setup() {

	Serial.begin(9600);
	// Configuration des LED des couleurs identificatives
	GreenLEDs[0].Configure(PIN_GREEN_LED1, PIN_GREEN_WHITE);
	GreenLEDs[1].Configure(PIN_GREEN_LED2, PIN_GREEN);
	GreenLEDs[2].Configure(PIN_GREEN_LED3, PIN_ORANGE_WHITE);
	GreenLEDs[3].Configure(PIN_GREEN_LED4, PIN_ORANGE);

	//Configuration des sorties des LEDs.
	for (int i = 0; i < 4; i++)
		GreenLEDs[i].Initialisation();
	

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

	analogWrite(PIN_BLUE, 225);
	analogWrite(PIN_BLUE_WHITE, 225);
	analogWrite(PIN_MARRON, 225);
	analogWrite(PIN_MARRON_WHITE, 225);
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
				IdLED = 0;
				Temp.startTimer(TimeDelayLED);
			}
		}
	}

	if (ledState){
		// C'est à ce niveau que doit se faire les opération sur le Test du cable Ethernet.
		// Car toutes les condition sont réunies.
		// set the LED: 
		// ActiveCableEthernet();
		SelectLED(IdLED); // On allume la LED correspondante.
		SendSignal(IdLED);
		if (Temp.isTimerReady()){
			if (IdLED == 3)
				IdLED = 0;
			else
				IdLED += 1;			
			Temp.startTimer(TimeDelayLED);
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
}



void ActiveCableEthernet(short int PinBroche){
	digitalWrite(PinBroche, HIGH);
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

void SendSignal(short int ExceptPin){
	for (int i = 0; i < 4; i++)
	{
		if (i != ExceptPin)
			analogWrite(GreenLEDs[i].GetPINLed(), 0);
		else
			analogWrite(GreenLEDs[i].GetPINLed(), 255);	
	}
	
}

void SelectLED(short int NumLED){
	switch (NumLED)
	{
	case 0:
		digitalWrite(PIN_GREEN_LED1, 1);
		digitalWrite(PIN_GREEN_LED2, 0);
		digitalWrite(PIN_GREEN_LED3, 0);
		digitalWrite(PIN_GREEN_LED4, 0);
		break;
	case 1:
		digitalWrite(PIN_GREEN_LED1, 0);
		digitalWrite(PIN_GREEN_LED2, 1);
		digitalWrite(PIN_GREEN_LED3, 0);
		digitalWrite(PIN_GREEN_LED4, 0);
		break;
	case 2:
		digitalWrite(PIN_GREEN_LED1, 0);
		digitalWrite(PIN_GREEN_LED2, 0);
		digitalWrite(PIN_GREEN_LED3, 1);
		digitalWrite(PIN_GREEN_LED4, 0);
		break;
	
	case 3:
		digitalWrite(PIN_GREEN_LED1, 0);
		digitalWrite(PIN_GREEN_LED2, 0);
		digitalWrite(PIN_GREEN_LED3, 0);
		digitalWrite(PIN_GREEN_LED4, 1);
		break;
	default:
		break;
	}
}

