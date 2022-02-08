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

/**
 * @brief Fonction qui envoi le signal suivant la representation :
 *      0 1 1 1 0 0 0 1 1
 * 
 */
void RJ45_Female::SendSignal(){
        analogWrite(PinCables[0], 0);
        analogWrite(PinCables[1], 255);
        analogWrite(PinCables[2], 255);
        analogWrite(PinCables[3], 255);
        analogWrite(PinCables[4], 0);
        analogWrite(PinCables[5], 0);
        analogWrite(PinCables[6], 255);
        analogWrite(PinCables[7], 255);

        // uint8_t Temp;
        // for (int i = 0; i < 8; i++)
        // {
        //     Temp = pow(2, i);
        //     if
        // }
        
}