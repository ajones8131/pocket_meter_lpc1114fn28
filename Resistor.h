/* Resistor.h */

#include "mbed.h"
#include <SPI.h>
#define address 0x11 /* Used to write to digipot, need to write this value before it accepts actual value*/

/* Tunable to get better measurements */

#define totalPotRes 89450 /* total resistance of digipot */
#define potWiperRes 210 /* Base resistance through digipot wiper when set to 0 */
#define muxRes 8 /* ohms of resistance through the switch being used */
#define Vin 3.3 /* Voltage in used in calculations */

/* -----------------END-------------- */

class Resistor
{
public:
    Resistor(SPI&, DigitalOut& , AnalogIn&);
    float measure();

private:
    SPI& spi;
    DigitalOut& resCS;
    AnalogIn& resMeas;

    int binarySearch(int, int);
    void setPot(int);
    float getPotVal(int);
};
/* End Resistor.h */
