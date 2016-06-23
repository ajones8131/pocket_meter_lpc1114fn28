/* Capacitor.cpp */

#include "Capacitor.h"

/* Constructor for Class */
/* Sets data pin and timer object */
Capacitor::Capacitor(DigitalIn& _dataIn, Timer& _timer)
    :dataIn(_dataIn), timer(_timer)
{}

/* Public Functions */

float Capacitor::measure(int resA, int resB, int timeout)
{
    int period;
    float value;

    period = getData(timeout);
    if(period != 0) {
        value = calculateValue(resA, resB, period);
    } else {
        value = 0;
    }
    return value;
}

/* Private Fuctions */

int Capacitor::getData(int timeout)
{
    int start, current, previousState, currentState, count, timeStamp[2], period;
    count = timeStamp[1] = timeStamp[0] = 0;
    start = timer.read_us();

    currentState = dataIn.read();
    previousState = currentState;

    while((timeStamp[1] == 0) && ((current - start) < timeout)) {
        current = timer.read_us();
        currentState = dataIn.read();
        if((currentState != previousState)) {
            timeStamp[count] = current;
            count++;
        }
        previousState = currentState;
    }

    if(timeStamp[1] == 0)
        return 0;

    period = timeStamp[1] - timeStamp[0];

    if(((period%2) == 1) && (currentState == 0))
        period -= 1;
    else if(((period%2) == 0) && (currentState == 1))
        period -= 1;


    return period;
}

//returns value of capacitor

float Capacitor::calculateValue(int resA, int resB, int period)
{
    float value, divisor;

    divisor = .693;

    if((period%2) == 0)
        divisor = resA + resB;
    else if(((period%2) == 1) && (resB == 685))
        divisor = .822*resB;
    else
        divisor = resB;

    value = period/(.693*divisor); //Calculate in uF

    return value;
}

/* End Capacitor.cpp */
