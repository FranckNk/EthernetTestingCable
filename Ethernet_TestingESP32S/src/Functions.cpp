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
        for (int i = 0; i < 8; i++)
        {
            if (analogRead(PinCables[i]) == 4095)
                SignalGot[i] = 1;
            else if (analogRead(PinCables[i]) == 0)
                SignalGot[i] = 0;
            // else
            //     return ; // le cable n'est pas branche ou il y'a un fil qui ne
            //     /// fonctionne pas correctement.
        }
        return true;
    }
bool RJ45_Female::TestCable(){
        for (int i = 0; i < 8; i++)
        {
            if (analogRead(PinCables[i]) != 4095 && analogRead(PinCables[i]) != 0)
                return false;
        }
        return true;
    }
void RJ45_Female::GetValues(short int Tab[]){
    for (int i = 0; i < 8; i++)
    {
        Tab[i] = digitalRead(PinCables[i]);
    }
    
}