// En même temps, on inclu Arduino.hvoid SelectLED(short int NumLED);
#ifndef FUNCTIONS_H
    #define FUNCTIONS_H

class RJ45_Female
{
private:
    /* data */
    short int PinCables[8];
    short int SignalGot[8];
    u_int8_t ValeurFinale = 0x00;
    u_int8_t ValCableDroit = 0x73;
    u_int8_t ValCableCroise = 0x97;
    u_int8_t ValCableCroiseInverse = 0x6E;
public:
    RJ45_Female();

    //Initialisation des broches.
    void InitialisationRJ45F();

    void SetPinsRJ45(short int PINS[]);

    void SendSignal();

    
};

 
#endif