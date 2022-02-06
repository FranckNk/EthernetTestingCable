// En même temps, on inclu Arduino.hvoid SelectLED(short int NumLED);
#ifndef FUNCTIONS_H
    #define FUNCTIONS_H

class RJ45_Female
{
private:
    /* data */
    short int PinCables[8];
    short int SignalGot[8];
    unsigned char ValeurFinale = ((unsigned char) 0x00);
    unsigned char ValCableDroit = ((unsigned char) 0x73);
    unsigned char ValCableCroise = ((unsigned char) 0x97);
    unsigned char ValCableCroiseInverse = ((unsigned char) 0x6E);
public:
    RJ45_Femalle();

    //Initialisation des broches.
    void InitialisationRJ45F(){
        for (int i = 0; i < sizeof(PinCables)/sizeof(short int); i++)
        {
            pinMode(PinCables[i], INPUT);
        }
        
    }

    void SetPinsRJ45(short int PINS[]){
        for (int i = 0; i < 8; i++)
        {
           PinCables[i] = PINS[i];
        }
    }

    bool GetSignal(){
        for (int i = 0; i < 8; i++)
        {
            if (analogRead(PinCables[i]) == 4095)
                SignalGot[i] = 1;
            else if (analogRead(PinCables[i]) == 0)
                SignalGot[i] = 0;
            else
                return false;
        }
        return true;
    }
};

 
#endif