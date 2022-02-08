/*

TITRE          : Test de 02 Cables RJ45 ESP32
AUTEUR         : Franck Nkeubou Awougang
DATE           : 28/01/2022
DESCRIPTION    : Programme pour tester deux cables RJ45, ce uC sera considéré comme éméteur du 
				 signal.
				 Inspiration du code BuitinExample de Arduino pour le rebondissement. Source : https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
				 Et aussi du code pour tester l'écran LCD.
VERSION        : 0.0.2

J'aimerai préciser que la vérification suit le principe binaire comme dans le
cours. Etant donné que je n'utilise pas les 1 ou 0 pour envoyer mon signal, alors
je ne peux pas fonctionner avec les valeurs 0 ou 1 comme signal.

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

// Déclaration des variables.
short int ModeCroise;
short int ModeDroit;

unsigned long TimeDelay 	= 3000;
unsigned long TimeDelayPrint 	= 2000;

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
	
	ModeDroit = digitalRead(PIN_MODE_DROIT);
	ModeCroise = digitalRead(PIN_MODE_CROISE);


	if (ModeDroit && !ModeCroise && MaTeteFemelle.GetSignal()){
		// set the LED:
		// Nous sommes au mode DROIT.
		if (Temp.isTimerReady()){
			if (MaTeteFemelle.CheckCableDroit()){
				Serial.print("\nMODE DROIT CORRECT.......\n");
			}
			else{
				Serial.print("\nMODE DROIT INCORRECT.......\n");
			}
			Serial.println(MaTeteFemelle.GetValueFinal());
			// short int* Tab;
			// Tab = MaTeteFemelle.GetSignalGot();
			// for (int i = 0; i < 8; i++)
			// {
			// 	Serial.print(Tab[i]);
			// }
			Serial.print("\nMODE DROIT\n\n");
			for (int i = 0; i < 8; i++)
			{
				Serial.print("Valeur PIN : ");
				Serial.print(i+1);
				Serial.print(" : ");
				Serial.println(analogRead(PinsValues[i]));
			}
			Serial.print("Valeur ModeDroit : ");
				Serial.println(ModeDroit);
			Serial.print("Valeur ModeCroise : ");
				Serial.println(ModeCroise);
			Temp.startTimer(TimeDelay);
		}
		
	}
	else if (!ModeDroit && ModeCroise && MaTeteFemelle.GetSignal()){
		// Nous sommes au Mode CROISE.
		if (Temp.isTimerReady()){
			if (MaTeteFemelle.CheckCableCroise()){
				Serial.print("\nMODE CROISE CORRECT.......\n");
			}
			else{
				Serial.print("\nMODE CROISE INCORRECT.......\n");
			}
			Serial.println(MaTeteFemelle.GetValueFinal());
			// short int* Tab;
			// Tab = MaTeteFemelle.GetSignalGot();
			// for (int i = 0; i < 8; i++)
			// {
			// 	Serial.print(Tab[i]);
			// }
			
			Serial.print("\nMODE CROISE\n\n");
			for (int i = 0; i < 8; i++)
			{
				Serial.print("Valeur PIN : ");
				Serial.print(i+1);
				Serial.print(" : ");
				Serial.println(analogRead(PinsValues[i]));
			}
			Serial.print("Valeur ModeDroit : ");
				Serial.println(ModeDroit);
			Serial.print("Valeur ModeCroise : ");
				Serial.println(ModeCroise);

			Temp.startTimer(TimeDelay);
		}
		
		
	}
	else{
		// La vérification est terminée... On affiche un message dans le LCD.
		// On fait maintenant clignoter l'écran LCD, changer la couleur
		if (Temp.isTimerReady()){
			Serial.print("\nAUCUN MODE CORRECT.......\n\n");
			if (millis() % 500 > 250) {
                lcd.setRGB(200, 0, 0);
            } else {
                lcd.setRGB(200, 200, 200);
            }
			Temp.startTimer(TimeDelay);
		}
		
		
	}
}
