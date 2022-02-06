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

// Déclaration des constantes.
// Les LED des modes.
const short int PIN_MODE_DROIT = 1;
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

// Déclaration des LED des couleurs identificatives. 
// LED ON lorsque le signal est envoyé.
LedFromColor GreenLEDs[4];

// Déclaration des variables.
// Variables will change:
bool FinalStatus = false;
bool CheckingStatus = true;
bool Finished = false;
short int ComptChecking = 0;
short int LastLED = 5;

short int StateResult[4] = {0};
short int ledState = LOW;         // L'état de cette variable va nous permettre de 
									// connaitre le mode de fonctionnement.
									// LOW = Mode DROIT ; HIGH = Mode CROISE
short int buttonState;             // the current reading from the input pin
short int lastButtonState = HIGH;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

short int PinSwitchMode 	= 19;		
short int IdLED = 0;
unsigned long TimeDelay 	= 500;
unsigned long TimeDelayPrint 	= 2000;

void SelectLED(short int NumLED);
short int GetSignalPin();
void TurnOffLEDs();
void TurnOnLEDs();
bool CableDecisionDroit();
bool CableDecisionCrosie();
void ResetResults();

void setup() {

	Serial.begin(9600);
	// Configuration des LED des couleurs identificatives
	GreenLEDs[0].Configure(PIN_GREEN_LED1, PIN_MARRON);
	GreenLEDs[1].Configure(PIN_GREEN_LED2, PIN_MARRON_WHITE);
	GreenLEDs[2].Configure(PIN_GREEN_LED3, PIN_ORANGE);
	GreenLEDs[3].Configure(PIN_GREEN_LED4, PIN_ORANGE_WHITE);

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

	TempPrint.startTimer(3000);
	
	  /**
     *  @brief initialize the LCD and master IIC
     */ 
    lcd.init();
    // Print a first message to the LCD.
    if (ledState){
		lcd.clear();
		lcd.setRGB(0, 0, 200);
		lcd.setCursor(0,0);
		lcd.print("Mode: DROIT");
		lcd.setCursor(0,1);
		lcd.print("Checking Cable..");
	}
	else{
		lcd.clear();
		lcd.setRGB(255, 255, 0);
		lcd.setCursor(0,0);
		lcd.print("Mode: CROISE");
		lcd.setCursor(0,1);
		lcd.print("Checking Cable..");
	}
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
				ledState = !ledState; // On change de mode de fonctionnement.
				// On reset toutes les variables pour le statut du mode.
				ComptChecking = 0;
				ResetResults();
				CheckingStatus = true;
				FinalStatus = false;
				Finished = false;
				LastLED = 5;
				IdLED = 0;
				// Affichage du mode dans l'écran LCD
				if (ledState){
					lcd.clear();
					lcd.setRGB(0, 0, 200);
					lcd.setCursor(0,0);
					lcd.print("Mode: DROIT");
					lcd.setCursor(0,1);
					lcd.print("Checking Cable..");
				}
				else{
					lcd.clear();
					lcd.setRGB(255, 255, 0);
					lcd.setCursor(0,0);
					lcd.print("Mode: CROISE");
					lcd.setCursor(0,1);
					lcd.print("Checking Cable..");
				}
				Temp.startTimer(TimeDelay);  // Temps pour allumer les LED.
				// TempPrint.startTimer(TimeDelayPrint);// Temps d'attente pour afficher
									// au moniteur série ou a l'écran LCD.
			}
		}
	}

	if (ledState && CheckingStatus){
		// set the LED:
		// Nous sommes au mode DROIT.

		IdLED = GetSignalPin();
		if (IdLED != 20){
			SelectLED(IdLED);	
			// Temp.startTimer(TimeDelay);
		}
		else
			TurnOffLEDs();
		
	}
	else if (!ledState && CheckingStatus){
		// Nous sommes au Mode CROISE.
		
		IdLED = GetSignalPin();
		if (IdLED != 20){
			SelectLED(IdLED);	
			// Temp.startTimer(TimeDelay);
		}
		else{
			TurnOffLEDs();
		}
		
	}
	else{
		// La vérification est terminée... On affiche un message dans le LCD.
		// On fait maintenant clignoter l'écran LCD, changer la couleur
		if (FinalStatus == true && GetSignalPin() != 20){
			if (millis() % 1000 > 500) {
                lcd.setRGB(0, 0, 200);
				TurnOnLEDs();
            } else {
                lcd.setRGB(200, 200, 0);
				TurnOffLEDs();
            }
		}
		else if (FinalStatus == false && GetSignalPin() != 20)
		{
			TurnOffLEDs();
			if (millis() % 500 > 250) {
                lcd.setRGB(200, 0, 0);
            } else {
                lcd.setRGB(200, 200, 200);
            }
		}
	}


	// save the reading. Next time through the loop, it'll be the lastButtonState:
	lastButtonState = reading;
}

short int GetSignalPin(){
	for (short i = 0; i < 4; i++)
	{
		if (analogRead(GreenLEDs[i].GetPINLed()) == 4095){
			
			return i;
		}
	}
	return 20;
	
}

void ResetResults(){
	for (int i = 0; i < 4; i++)
	{
		StateResult[i] = 5;
	}
	
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
		StateResult[ComptChecking] = GreenLEDs[NumLED].GetPINLed();
		ComptChecking += 1;
	}else if (ComptChecking == 4){
		CheckingStatus = false;
		Finished = true;
		FinalStatus = ledState? CableDecisionDroit():CableDecisionCroise();
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
