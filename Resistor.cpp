/* Resistor.cpp */

#include "Resistor.h"

/* Constructor for Class
 * Sets spi, and analog read
 * initializes a value for Vin */
Resistor::Resistor(SPI& _spi, DigitalOut& _resCS, AnalogIn& _resMeas)
    :spi(_spi), resCS(_resCS), resMeas(_resMeas)
{}

/* Public Functions */

/* Takes one measurment
 * Returns measured resistance in ohms */
float Resistor::measure()
{
    float Vout, potValue;
    int potInt;
    Vout = 0;
    wait(.001);
    potInt = binarySearch(0, 256);
    setPot(potInt);
    wait(.001);
    potValue = getPotVal(potInt);
    for(int i = 0; i < 64; i++) {
        Vout += resMeas.read();
    }
    Vout *= 3.3;
    Vout /= 64;
    return (potValue*Vout)/(Vin - Vout);
}

/* Private Functions */


/* Finds the correct resistance to get an accurate measurement with the digital pot, pass in 0 and # of taps for pot*/
int Resistor::binarySearch(int low, int high)
{
    float Vout;
    int value, mid = (high+low)/2;
    setPot(mid);
    wait(.001);
    Vout = resMeas.read()*Vin;

    if((mid == 0 || mid == 255))
        Vout = .5*Vin;

    if(Vout > (.6*Vin))
        value = binarySearch(mid, high);
    else if(Vout < (.3*Vin))
        value = binarySearch(low, mid);
    else
        value = mid;

    return value;
}

/* Takes a value 0-255 and sets the "100k" pot value, this pot is 89.4k */
/* It usually take 1ms for stable output */
void Resistor::setPot(int value)
{
    resCS = 0;
    spi.write(address);
    spi.write(value);
    resCS = 1;
}

/* Takes a value 0-255 and returns the "100k" pot value, this pot is 89.5k */
float Resistor::getPotVal(int value)
{
    return(((totalPotRes-potWiperRes)*value)/256)+potWiperRes + muxRes;
}
/* End Resistor.cpp */
