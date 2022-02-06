#include <Arduino.h>
#include "Functions.h"

RJ45_Female::RJ45_Female(){};
void RJ45_Female::InitialisationRJ45F(){
        for (int i = 0; i < 8; i++)
        {
            pinMode(PinCables[i], OUTPUT);
        }
        
    }
void RJ45_Female::SetPinsRJ45(short int PINS[]){
        for (int i = 0; i < 8; i++)
        {
           PinCables[i] = PINS[i];
        }
    }
void RJ45_Female::SendSignal(){
        for (int i = 0; i < 8; i++)
        {
            analogWrite(PinCables[i], 255);
            // else
            //     return ; // le cable n'est pas branche ou il y'a un fil qui ne
            //     /// fonctionne pas correctement.
        }
    }