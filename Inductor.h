/* Inductor.h */

#include <InterruptIn.h>
#include <Timer.h>
#include <DigitalOut.h>
#include "mbed.h"
#define pi  3.14159265359

/* Tunable to get better measurements */
#define capacitance .000001 //1 uF

/* -----------------END-------------- */

class Inductor
{
public:
    Inductor(DigitalIn&, Timer&, DigitalOut&);//1 interrupt
    float measure();

private:
    DigitalIn& dataIn;
    Timer& timer;
    DigitalOut& pwmOut;

    float getData(int);

    long timeStamp[2];
    int count;
};
/* End Inductor.h */
