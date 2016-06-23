/* main.h */
#include "mbed.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "Inductor.h"
#include "LCD.h"
#include <SPI.h>
#include <Timer.h>
#include <DigitalOut.h>


/* Pins for Switch */
/* Pins */
DigitalOut selectCap(dp25);//Capacitor      C
DigitalOut selectInd(dp28);//Inductor      L
DigitalOut selectRes(dp26);//Resistor     R

/* SPI for LCD and Resistor SS Pins declared elsewhere */
SPI spi(dp2, dp1, dp6);//mosi, miso, sck

//Timer for capacitor and inductor
Timer timer;

/* Pins for LCD */
DigitalOut dcPin(dp10);
DigitalOut scePin(dp9);
LCD lcd(spi, dcPin, scePin);

/* Pins for Resistor Circuit */
DigitalOut resCS(dp1);// select pin for digitalPot
AnalogIn resAin(dp4); // pin for analog read of resistor circuit
Resistor resistor(spi, resCS, resAin);

/* Pins for Capacitor Circuit */
DigitalIn capIn(dp24, PullDown);
Capacitor capacitor(capIn, timer);
/* Pins for Capacitor Resistor Select */
DigitalOut resOne(dp14); // 10K Ohm
DigitalOut resTwo(dp16); // 470 Ohm

/* Pins for Inductor Circuit */
DigitalOut pwmPin(dp11);
DigitalIn indIn(dp13);
Inductor inductor(indIn, timer, pwmPin);

/* Methods in Main */
int cap();
int ind();
int res();

/* End main.h */