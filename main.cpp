/* main.cpp */
#include "main.h"

int main()
{
    int val = 0;
    timer.start();
    selectCap = selectInd = selectRes = 0;
    resOne = resTwo = 0;

    while(1) {
        if(!ind()) {
                if(!cap()) {
                    if(!res()) {
                        lcd.setScreenNone();
                    }
                }
        }
        wait(.1);
    }
}

int cap()
{
    float value;
    char mesg[10];
    selectCap = 1;
    wait(.01);
    value = capacitor.measure(1000675, 1000675, 50000);
    if(value == 0) {
        resOne = 1;
        wait(.001);
        value = capacitor.measure(10675, 10675, 50000);
        if(value == 0) {
            resOne = 0;
            resTwo = 1;
            wait(.001);
            value = capacitor.measure(685, 685, 1500000);
        }
    } else if(value < .010715) {
        for(int i = 1; i < 10; i++) {
            value += capacitor.measure(1000675, 1000675, 50000);
        }
        value /= 10;
    }

    selectCap = resOne = resTwo = 0;
    if(value > .000715) {
        lcd.setScreenCap(value - .000700);
        return 1;
    } else {
        return 0;
    }


}

int ind()
{
    float value;
    selectInd = 1;
    wait(.01);
    value = inductor.measure();
    if(value != 0) {
        for(int i = 1; i < 10; i++) {
            value += inductor.measure();
        }
        value /= 10;
        lcd.setScreenInd(value);
        selectInd = 0;
        return 1;
    }
    selectInd = 0;
    return 0;
}
int res()
{
    float value[30], maxValue = 0, avgVal = 0, cap = 1;
    selectRes = 1;
    for(int i = 0; i < 30; i++) {
        value[i] = resistor.measure();
        if(value[i] >= maxValue)
            maxValue = value[i];
        else
            cap = 0;

        avgVal += value[i];
        wait(.01);
    }
    avgVal /= 30;
    if(cap)
    return 0;
    if ((avgVal > 2000000) ||(avgVal <=0))
        return 0;
    avgVal = resistor.cal(avgVal);
    lcd.setScreenRes(avgVal);
    return 1;
}
/* End main.cpp */
