/* LCD.h */
#include <SPI.h>
#include <PwmOut.h>

#include "mbed.h"

/* PCD8544-specific defines: */
#define LCD_COMMAND  0
#define LCD_DATA     1

#define LOW     0
#define HIGH    1

/* 84x48 LCD Defines: */
#define LCD_WIDTH   84 // Note: x-coordinates go wide
#define LCD_HEIGHT  48 // Note: y-coordinates go high
#define WHITE       0  // For drawing pixels. A 0 draws white.
#define BLACK       1  // A 1 draws black.

typedef unsigned char       byte;
typedef unsigned int        boolean;


class LCD
{
public:
    LCD(SPI&, DigitalOut&, DigitalOut&);
    void setPixel(int, int);
    void clearPixel(int, int);
    void setPixel(int, int, boolean);
    void setChar(char, int, int, boolean);
    void setStr(char *, int, int, boolean);
    void setBitmap(const char *);
    void clearDisplay(boolean );
    void setContrast(byte);
    void gotoXY(int , int );
    void updateDisplay();
    void invertDisplay();
    void LCDWrite(byte, byte);
    void lcdBegin();
    void setScreenCap(float);
    void setScreenInd(float);
    int setScreenRes(float);
    void setScreenNone();
    char rangePrefix(float, int);
    float rangeValue(float, int);
//        ~LCD();

private:
    SPI& spi;
    DigitalOut& dcPin;
    DigitalOut& scePin;

};

/* End LCD.h */