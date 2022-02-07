// En même temps, on inclu Arduino.hvoid SelectLED(short int NumLED);
#ifndef FUNCTIONS_H
    #define FUNCTIONS_H

class RJ45_Female
{
private:
    /* data */
    short int PinCables[8];
    short int SignalGot[8];
    uint8_t ValeurFinale = 0x00;
    uint8_t ValCableDroit = 0xCE;
    uint8_t ValCableCroise = 0x97;
    uint8_t ValCableCroiseInverse = 0x6E;
public:
    RJ45_Female();

    //Initialisation des broches.
    void InitialisationRJ45F();

    void SetPinsRJ45(short int PINS[]);

    bool GetSignal();

    bool TestCable();

    void SetBitHigh(int Pos);

    void SetBitLow(int Pos);

    bool CheckCableDroit();

    bool CheckCableCroise();

    uint8_t GetValueFinal(){
        return ValeurFinale;
    }
    short int* GetSignalGot(){
        return SignalGot;
    }

};

 
#endif