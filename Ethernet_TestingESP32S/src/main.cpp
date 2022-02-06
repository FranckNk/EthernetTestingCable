/*

TITRE          : Test de 02 Cables RJ45 ESP32
AUTEUR         : Franck Nkeubou Awougang
DATE           : 28/01/2022
DESCRIPTION    : Programme pour tester deux cables RJ45, ce uC sera considéré comme éméteur du 
				 signal.
				 Inspiration du code BuitinExample de Arduino pour le rebondissement. Source : https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
				 Et aussi du code pour tester l'écran LCD.
VERSION        : 0.0.1

*/

// Librairies.
#include <Arduino.h>
#include "DFRobot_RGBLCD1602.h"
#include "Timer.h"
#include "Functions.h"

// Déclaration de la classe LCD pour notre afficheur.
DFRobot_RGBLCD1602 lcd(/*lcdCols*/16,/*lcdRows*/2);  //16 characters and 2 lines of show 

// Declaration d'une instance de la classe Timer.
Timer Temp;
Timer TempPrint;
RJ45_Female MaTeteFemelle;

// Déclaration des constantes.
// Les LED des modes.
const short int PIN_MODE_DROIT = 18;
const short int PIN_MODE_CROISE = 4;

// Les LEDs.
const short int PIN_GREEN_LED1 = 2;
const short int PIN_GREEN_LED2 = 5;
const short int PIN_GREEN_LED3 = 3;
const short int PIN_GREEN_LED4 = 23;
// Les Pin pour le RJ45 Femelle.
const short int PIN_BLUE     	 = 26;
const short int PIN_GREEN        = 14;
const short int PIN_ORANGE  	 = 33;
const short int PIN_MARRON  	 = 13;
const short int PIN_BLUE_WHITE 	 = 27;
const short int PIN_GREEN_WHITE  = 25;
const short int PIN_ORANGE_WHITE = 32;
const short int PIN_MARRON_WHITE = 12;

// Declaration du tableau de toutes les pins du RJ45_Female
short int PinsValues[8] = {PIN_ORANGE_WHITE, PIN_ORANGE, PIN_GREEN_WHITE, PIN_BLUE, 
							PIN_BLUE_WHITE, PIN_GREEN, PIN_MARRON_WHITE, PIN_MARRON};
short int GotValues[8] = {0};

// Déclaration des variables.
// Variables will change:
bool FinalStatus = false;
bool CheckingStatus = true;
bool Finished = false;
short int ComptChecking = 0;
short int LastLED = 5;
short int ModeCroise;
short int ModeBlue;

short int StateResult[4] = {0};
unsigned long TimeDelay 	= 3000;
unsigned long TimeDelayPrint 	= 2000;

void TurnOffLEDs();
void TurnOnLEDs();
bool CableDecisionDroit();
bool CableDecisionCrosie();
void ResetResults();

void setup() {

	Serial.begin(9600);

	pinMode(PIN_GREEN_LED1, OUTPUT);
	pinMode(PIN_GREEN_LED2, OUTPUT);
	pinMode(PIN_GREEN_LED3, OUTPUT);
	pinMode(PIN_GREEN_LED4, OUTPUT);

	pinMode(PIN_MODE_CROISE, INPUT);
	pinMode(PIN_MODE_DROIT, INPUT);

	// Configuration des sorties des broches du RJ45 Femalle.
	MaTeteFemelle.SetPinsRJ45(PinsValues);
	MaTeteFemelle.InitialisationRJ45F();

	// Configuration de la broche de décision.

	Temp.startTimer(4000);
	
	  /**
     *  @brief initialize the LCD and master IIC
     */ 
    lcd.init();
    // Print a first message to the LCD.
		lcd.clear();
		lcd.setRGB(0, 0, 200);
		lcd.setCursor(0,0);
		lcd.print("Mode: NONE");
		lcd.setCursor(0,1);
		lcd.print("Starting..");
	
	digitalWrite(PIN_GREEN_LED1, HIGH);
}

void loop() {
	// put your main code here, to run repeatedly:
	
	ModeBlue = digitalRead(PIN_MODE_DROIT);
	ModeCroise = digitalRead(PIN_MODE_CROISE);


	if (ModeBlue && !ModeCroise){
		// set the LED:
		// Nous sommes au mode DROIT.
		MaTeteFemelle.GetSignal();
		MaTeteFemelle.GetValues(GotValues);
		if (Temp.isTimerReady()){
			Serial.print("\nMODE DROIT\n\n");
			for (int i = 0; i < 8; i++)
			{
				Serial.print("Valeur PIN : ");
				Serial.print(i+1);
				Serial.print(" : ");
				Serial.println(analogRead(PinsValues[i]));
			}
			Serial.print("Valeur ModeBlue : ");
				Serial.println(ModeBlue);
			Serial.print("Valeur ModeCroise : ");
				Serial.println(ModeCroise);
			Temp.startTimer(TimeDelay);
		}
		
	}
	else if (!ModeBlue && ModeCroise){
		// Nous sommes au Mode CROISE.
		MaTeteFemelle.GetSignal();
		MaTeteFemelle.GetValues(GotValues);
		if (Temp.isTimerReady()){
			Serial.print("\nMODE CROISE\n\n");
			for (int i = 0; i < 8; i++)
			{
				Serial.print("Valeur PIN : ");
				Serial.print(i+1);
				Serial.print(" : ");
				Serial.println(analogRead(PinsValues[i]));
			}
			Serial.print("Valeur ModeBlue : ");
				Serial.println(ModeBlue);
			Serial.print("Valeur ModeCroise : ");
				Serial.println(ModeCroise);
			Temp.startTimer(TimeDelay);
		}
		
		
	}
	else{
		// La vérification est terminée... On affiche un message dans le LCD.
		// On fait maintenant clignoter l'écran LCD, changer la couleur
		if (FinalStatus == true && MaTeteFemelle.TestCable()){
			if (millis() % 1000 > 500) {
                lcd.setRGB(0, 0, 200);
				TurnOnLEDs();
            } else {
                lcd.setRGB(200, 200, 0);
				TurnOffLEDs();
            }
		}
		else if (FinalStatus == false && MaTeteFemelle.TestCable())
		{
			TurnOffLEDs();
			if (millis() % 500 > 250) {
                lcd.setRGB(200, 0, 0);
            } else {
                lcd.setRGB(200, 200, 200);
            }
		}
	}
}

short int GetSignalPin(){
	for (short i = 0; i < 4; i++)
	{
		
	}
	return 20;
	
}

bool CableDecisionDroit(){
	if (StateResult[0] == PIN_MARRON && StateResult[1] == PIN_MARRON_WHITE && StateResult[2] == PIN_ORANGE && StateResult[3] == PIN_ORANGE_WHITE )
	{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Mode: DROIT");
		lcd.setCursor(0,1);
		lcd.print("Good Cable");
		return true;
	}
	else{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Mauvais Cable :(");
		lcd.setCursor(0,1);
		lcd.print("Check MODE...");
		return false;
	}
}

bool CableDecisionCroise(){
	if (StateResult[0] == PIN_ORANGE && StateResult[1] == PIN_ORANGE_WHITE && StateResult[2] == PIN_MARRON && StateResult[3] == PIN_MARRON_WHITE )
	{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Mode: CROISE");
		lcd.setCursor(0,1);
		lcd.print("Good Cable");
		return true;
	}
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Mauvais Cable :(");
	lcd.setCursor(0,1);
	lcd.print("Check MODE...");
	return false;
}

void SelectLED(short int NumLED){
	
	if (CheckingStatus == true && NumLED != LastLED && ComptChecking < 4){
		LastLED = NumLED;
		// StateResult[ComptChecking] = GreenLEDs[NumLED].GetPINLed();
		ComptChecking += 1;
	}else if (ComptChecking == 4){
		CheckingStatus = false;
		Finished = true;
		// FinalStatus = ledState? CableDecisionDroit():CableDecisionCroise();
	}
	
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

void TurnOffLEDs(){
	digitalWrite(PIN_GREEN_LED1, 0);
	digitalWrite(PIN_GREEN_LED2, 0);
	digitalWrite(PIN_GREEN_LED3, 0);
	digitalWrite(PIN_GREEN_LED4, 0);
}

void TurnOnLEDs(){
	digitalWrite(PIN_GREEN_LED1, 1);
	digitalWrite(PIN_GREEN_LED2, 1);
	digitalWrite(PIN_GREEN_LED3, 1);
	digitalWrite(PIN_GREEN_LED4, 1);
}
