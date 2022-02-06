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
    RJ45_Female();

    //Initialisation des broches.
    void InitialisationRJ45F();

    void SetPinsRJ45(short int PINS[]);

    void SendSignal();
};

 
#endif