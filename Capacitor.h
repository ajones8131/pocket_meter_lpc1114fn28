/* Capacitor.h */
#include <InterruptIn.h>
#include <Timer.h>
#include <DigitalOut.h>
#include "mbed.h"

class Capacitor
{
public:
    Capacitor(DigitalIn&, Timer&);//1 interrupt
    float measure(int, int, int);

private:
    DigitalIn& dataIn;
    Timer& timer;

    int getData(int);
    float calculateValue(int, int, int);

};

/* End Capacitor.h */
