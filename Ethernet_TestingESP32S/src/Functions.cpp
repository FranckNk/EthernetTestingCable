#include <Arduino.h>
#include "Functions.h"

RJ45_Female::RJ45_Female(){};
void RJ45_Female::InitialisationRJ45F(){
        for (int i = 0; i < 8; i++)
        {
            pinMode(PinCables[i], INPUT);
        }
        
    }
void RJ45_Female::SetPinsRJ45(short int PINS[]){
        for (int i = 0; i < 8; i++)
        {
           PinCables[i] = PINS[i];
        }
    }
bool RJ45_Female::GetSignal(){
        ValeurFinale = 0x00;
        uint8_t temp;
        // Le premier bit du processeur est le bit de poid le plus faible. 
        // La boule commence par le bit de poids le plus fort.
        // On doit alors decrementer dans le
        for (int i = 7; i >= 0; i--)
        {
            temp = pow(2, i);
            if (analogRead(PinCables[i]) == 4095){
                SignalGot[i] = 1;
                ValeurFinale = (ValeurFinale | temp);
            }
            else if (analogRead(PinCables[i]) == 0){
                SignalGot[i] = 0;
                ValeurFinale = ValeurFinale & (~(temp));
            }
            else
                return false; // le cable n'est pas branche ou il y'a un fil qui ne
                /// fonctionne pas correctement.
        }
        return true;
    }

void RJ45_Female::SetBitHigh(int Pos){
        uint8_t temp = pow(2, Pos);
        ValeurFinale = (ValeurFinale ^ temp);
    }

void RJ45_Female::SetBitLow(int Pos){
        uint8_t temp = pow(2, Pos);
        ValeurFinale = ValeurFinale ^ (~(temp));
    }

bool RJ45_Female::CheckCableDroit(){
        // On se rassure qu'on verifie lorsque le cable est correct.
        if (ValeurFinale == ValCableDroit)
        {
            return true;
        }
        else return false;
    }
bool RJ45_Female::CheckCableCroise(){
        // On se rassure qu'on verifie lorsque le cable est correct.
        if (ValeurFinale == ValCableCroise || ValeurFinale == ValCableCroiseInverse)
        {
            return true;
        }
        else return false;
    }

