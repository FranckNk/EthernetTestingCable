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

    bool CheckCableDroit();

    bool CheckCableCroise();
    // Juste pour savoir quelle valeur le processeur a recu et son ordre de lecture.
    uint8_t GetValueFinal(){
        return ValeurFinale;
    }
    // Juste pour tester le signal recu et le signal compris par le processeur.
    short int* GetSignalGot(){
        return SignalGot;
    }

};

 
#endif