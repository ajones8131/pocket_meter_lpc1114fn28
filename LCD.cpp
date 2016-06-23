/* LCD.cpp */

#include "LCD.h"
#include "LCDFonts.h"
#include "LCDBitmaps.h"

/* Constructor for Class
 * Sets Interrupt */
LCD::LCD(SPI& _spi, DigitalOut& _dcPin, DigitalOut& _scePin)
    :spi(_spi), dcPin(_dcPin), scePin(_scePin)
{
    lcdBegin();
}



// Because I keep forgetting to put bw variable in when setting...
void LCD::setPixel(int x, int y)
{
    setPixel(x, y, BLACK); // Call setPixel with bw set to Black
}

void LCD::clearPixel(int x, int y)
{
    setPixel(x, y, WHITE); // call setPixel with bw set to white
}

// This function sets a pixel on displayMap to your preferred
// color. 1=Black, 0= white.
void LCD::setPixel(int x, int y, boolean bw)
{
    // First, double check that the coordinate is in range.
    if ((x >= 0) && (x < LCD_WIDTH) && (y >= 0) && (y < LCD_HEIGHT)) {
        byte shift = y % 8;

        if (bw) // If black, set the bit.
            displayMap[x + (y/8)*LCD_WIDTH] |= 1<<shift;
        else   // If white clear the bit.
            displayMap[x + (y/8)*LCD_WIDTH] &= ~(1<<shift);
    }
}


// This function will draw a char (defined in the ASCII table
// near the beginning of this sketch) at a defined x and y).
// The color can be either black (1) or white (0).
void LCD::setChar(char character, int x, int y, boolean bw)
{
    byte column; // temp byte to store character's column bitmap
    for (int i=0; i<5; i++) { // 5 columns (x) per character
        column = ASCII[character - 0x20][i];
        for (int j=0; j<8; j++) { // 8 rows (y) per character
            if (column & (0x01 << j)) // test bits to set pixels
                setPixel(x+i, y+j, bw);
            else
                setPixel(x+i, y+j, !bw);
        }
    }
}

// setStr draws a string of characters, calling setChar with
// progressive coordinates until it's done.
// This function was grabbed from the SparkFun ColorLCDShield
// library.
void LCD::setStr(char * dString, int x, int y, boolean bw)
{
    while (*dString != 0x00) { // loop until null terminator
        setChar(*dString++, x, y, bw);
        x+=5;
        for (int i=y; i<y+8; i++) {
            setPixel(x, i, !bw);
        }
        x++;
        if (x > (LCD_WIDTH - 5)) { // Enables wrap around
            x = 0;
            y += 8;
        }
    }
}

// This function will draw an array over the screen. (For now) the
// array must be the same size as the screen, covering the entirety
// of the display.
void LCD::setBitmap(const char * bitArray)
{
    for (int i=0; i<(LCD_WIDTH * LCD_HEIGHT / 8); i++)
        displayMap[i] = bitArray[i];
}

// This function clears the entire display either white (0) or
// black (1).
// The screen won't actually clear until you call updateDisplay()!
void LCD::clearDisplay(boolean bw)
{
    for (int i=0; i<(LCD_WIDTH * LCD_HEIGHT / 8); i++) {
        if (bw)
            displayMap[i] = 0xFF;
        else
            displayMap[i] = 0;
    }
}

// Helpful function to directly command the LCD to go to a
// specific x,y coordinate.
void LCD::gotoXY(int x, int y)
{
    LCDWrite(0, 0x80 | x);  // Column.
    LCDWrite(0, 0x40 | y);  // Row.  ?
}

// This will actually draw on the display, whatever is currently
// in the displayMap array.
void LCD::updateDisplay()
{
    gotoXY(0, 0);
    for (int i=0; i < (LCD_WIDTH * LCD_HEIGHT / 8); i++) {
        LCDWrite(LCD_DATA, displayMap[i]);
    }
}

// Set contrast can set the LCD Vop to a value between 0 and 127.
// 40-60 is usually a pretty good range.
void LCD::setContrast(byte contrast)
{
    LCDWrite(LCD_COMMAND, 0x21); //Tell LCD that extended commands follow
    LCDWrite(LCD_COMMAND, 0x80 | contrast); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
    LCDWrite(LCD_COMMAND, 0x20); //Set display mode
}

/* There are two ways to do this. Either through direct commands
to the display, or by swapping each bit in the displayMap array.
We'll leave both methods here, comment one or the other out if
you please. */
void LCD::invertDisplay()
{
    /* Direct LCD Command option
    LCDWrite(LCD_COMMAND, 0x20); //Tell LCD that extended commands follow
    LCDWrite(LCD_COMMAND, 0x08 | 0x05); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
    LCDWrite(LCD_COMMAND, 0x20); //Set display mode  */

    /* Indirect, swap bits in displayMap option: */
    for (int i=0; i < (LCD_WIDTH * LCD_HEIGHT / 8); i++) {
        displayMap[i] = ~displayMap[i] & 0xFF;
    }
    updateDisplay();
}

// There are two memory banks in the LCD, data/RAM and commands.
// This function sets the DC pin high or low depending, and then
// sends the data byte
void LCD::LCDWrite(byte data_or_command, byte data)
{
    //Tell the LCD that we are writing either to data or a command
    dcPin = data_or_command;

    //Send the data
    scePin = 0;
    spi.write(data); //shiftOut(sdinPin, sclkPin, MSBFIRST, data);
    scePin = 1;
}

//This sends the magical commands to the PCD8544
void LCD::lcdBegin()
{
    scePin = 1;

    spi.format(8, 0);
    spi.frequency(50000);

    LCDWrite(LCD_COMMAND, 0x21); //Tell LCD extended commands follow
    LCDWrite(LCD_COMMAND, 0xB8); //Set LCD Vop (Contrast)
    LCDWrite(LCD_COMMAND, 0x04); //Set Temp coefficent
    LCDWrite(LCD_COMMAND, 0x14); //LCD bias mode 1:48 (try 0x13)
    //We must send 0x20 before modifying the display control mode
    LCDWrite(LCD_COMMAND, 0x20);
    LCDWrite(LCD_COMMAND, 0x0C); //Set display control, normal mode.
    setBitmap(startupBytes);
    updateDisplay();
}


void LCD::setScreenCap(float num)
{
    char prefix, mesg[10];
    prefix = ' ';
    prefix = rangePrefix(num, 0);
    num = rangeValue(num, 0);
    setBitmap(capacitorBytes);
    setStr("Capacitor", 15, 0, BLACK);
    sprintf(mesg, "%.2f%cF", num, prefix);
    setStr(mesg, 15, 32, BLACK);
    updateDisplay();

}

void LCD::setScreenInd(float num)
{
    char prefix, mesg[10];
    prefix = rangePrefix(num, 0);
    num = rangeValue(num, 0);
    setBitmap(inductorBytes);
    setStr("Inductor", 15, 0, BLACK);
    sprintf(mesg, "%.2f%cH", num, prefix);
    setStr(mesg, 15, 32, BLACK);
    updateDisplay();
}

int LCD::setScreenRes(float num)
{
    char prefix, mesg[10];

    prefix = rangePrefix(num, 1);
    num = rangeValue(num, 1);

    setBitmap(resistorBytes);
    setStr("Resistor", 15, 0, BLACK);
    sprintf(mesg, "%.2f%c#", num, prefix);
    setStr(mesg, 15, 32, BLACK);
    updateDisplay();
    return 1;
}

void LCD::setScreenNone()
{
    clearDisplay(WHITE);
    setStr("No Component", 4, 0, BLACK);
    updateDisplay();
}

char LCD::rangePrefix(float value, int res)
{
    char prefix;

    if((value == 0) || (value == -1))
        return ' ';

    if(abs(value) >= 1000000) res ? prefix = 'M' : prefix = ' ';
    else if(abs(value) >= 1000) res ? prefix = 'k': prefix = 'm';
    else if(abs(value) >= 1) res ? prefix = ' ': prefix = 'u';

    else if(abs(value) <= .000001) prefix = 'f';
    else if(abs(value) <= .001) prefix = 'p';
    else if(abs(value) <= 1) res ? prefix = ' ': prefix = 'n';
    else prefix = ' ';
    return prefix;
}
//pass 1 for resistor, 0 for cap/ind
float LCD::rangeValue(float value, int res)
{
    if(value == 0)
        return 0;
    else if(value == -1)
        return -1;

    while(abs(value) > 1000) value /= 1000;
    if(!res) {
        while(abs(value) < 1)
            value *= 1000;
    }
    return value;
}

/* End LCD.cpp */