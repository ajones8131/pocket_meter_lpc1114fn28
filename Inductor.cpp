/* Inductor.cpp */

#include "Inductor.h"

/* Constructor for Class */
Inductor::Inductor(DigitalIn& _dataIn, Timer& _timer, DigitalOut& _pwmOut)
    :dataIn(_dataIn), timer(_timer), pwmOut(_pwmOut)
{}

/* Public Functions */

float Inductor::measure()
{
    float value, frequency;

    frequency = getData(10000);


    if(frequency != 0) {
        value = 1000000/(capacitance*4*pi*pi*frequency*frequency);// Calculate in uH
    } else {
        value = 0; //Case for non-inductor measured
    }

    return value;
}

/* Private Functions */

/* Accepts value in microseconds for timeout
 * Measurments have a maximum amount of time based on the max value of components
 * currently set for 1mH
 * Returns frequency of LC tank circuit
 */
float Inductor::getData(int timeout)
{
    int start, current, previousState, currentState, count, timeStamp[2], period;
    float frequency;
    count = timeStamp[1] = timeStamp[0] = 0;
    start = timer.read_us();

    pwmOut = 1;
    wait(.00025); /* wait 250 us for cap to charge */
    pwmOut = 0;

    currentState = dataIn.read();
    previousState = currentState;

    while((timeStamp[1] == 0) && ((current - start) < timeout)) {
        current = timer.read_us();
        currentState = dataIn.read();
        if((currentState != previousState) && (currentState == 1)) {
            timeStamp[count] = current;
            count++;
        }
        previousState = currentState;
    }

    if(timeStamp[1] == 0)
        return 0;

    period = (timeStamp[1] - timeStamp[0]);
    frequency = 1000000/period;

    return frequency;
}
/* End Inductor.cpp */
