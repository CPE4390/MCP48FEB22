#include <xc.h>
#include <stdio.h>
#include "LCD.h"
#include "MCP48FEB22.h"
#include <math.h>


#pragma config FOSC=HSPLL
#pragma config WDTEN=OFF
#pragma config XINST=OFF


void InitPins(void);
void ConfigInterrupts(void);

#define _XTAL_FREQ 40000000L

char lcdstr[17];

void main(void) {
    const double PI = 3.14159265;
    const double PI_2 = 2 * PI;
    double theta = 0.0;
    double step = PI / 25;
    int reg = 0;
    OSCTUNEbits.PLLEN = 1;
    InitPins();
    LCDInit();
    LCDClear();
    ConfigInterrupts();
    InitMCP48FEB22();
    reg = MCP48FEB22ReadRegister(0x0a);
    MCP48FEB22WriteRegister(0x08, 0x0000);  //VREF = Vdd
    WriteDAC(0, 0x0);
    WriteDAC(1, 0x0);
    LCDWriteLine("Starting", 0);
    int v0 = 0;
    int v1 = 0;
    while (1) {
        v0 = (int)(2047.5 * (sin(theta) + 1.0));
        WriteDAC(0, v0);
        theta += step;
        if (theta >= PI_2) {
            theta = 0.0;
        }
        WriteDAC(1, v1);
        v1 += 50;
        if (v1 >= 0x1000) {
            v1 = 0;
        }
    }
}

void InitPins(void) {
    LATD = 0;
    TRISD = 0;
    WDTCONbits.ADSHR = 1;
    ANCON0 = ANCON1 = 0xff;  //All digital
    WDTCONbits.ADSHR = 1;
}

void ConfigInterrupts(void) {

    INTCONbits.GIE = 1; //Turn on interrupts
}



void interrupt HighIsr(void) {
    
    //Check the source of the interrupt
    
}


