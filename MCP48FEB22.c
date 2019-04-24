#include <xc.h>

//Connections:
//	Master RD4 <-> MOSI
//	Master RD5 <-> MISO
//	Master RD6 <-> SCK
//	Master RD7 <-> CS


#define CS      LATDbits.LATD7   //Use RD7 as SS

void InitMCP48FEB22(void) {
    TRISD = 0b00100000; //MMSP2 uses RD4 as SDO, RD5 as SDI, RD6 as SCK
    CS = 1;
    SSP2STATbits.CKE = 1;
    SSP2CON1bits.CKP = 0; //SPI mode 0,0
    SSP2CON1bits.SSPM = 0b0000; //SPI Master - FOSC/16 = 2.5 MHz
    SSP2CON1bits.SSPEN = 1; //Enable MSSP
}

char MCP48FEB22WriteRegister(char reg, unsigned int value) {
    unsigned char rx;

    reg <<= 3;
    CS = 0;
    SSP2BUF = reg;
    while (SSP2STATbits.BF == 0); //Wait for tx/rx
    rx = SSP2BUF; //read received byte
    if (rx & 0x01) { //Command is valid
        SSP2BUF = value >> 8;
        while (SSP2STATbits.BF == 0);
        rx = SSP2BUF;
        SSP2BUF = value;
        while (SSP2STATbits.BF == 0);
        rx = SSP2BUF;
        CS = 1;
        return 1;
    }
    CS = 1;
    return 0;
}

unsigned int MCP48FEB22ReadRegister(char reg) {
    unsigned char rx;
    unsigned int value = 0xffff;

    reg <<= 3;
    reg |= 0b110;
    CS = 0;
    SSP2BUF = reg;
    while (SSP2STATbits.BF == 0); //Wait for tx/rx
    rx = SSP2BUF; //read received byte
    if (rx & 0x01) { //Command is valid
        SSP2BUF = 0;
        while (SSP2STATbits.BF == 0);
        value = SSP2BUF;
        value <<= 8;
        SSP2BUF = value;
        while (SSP2STATbits.BF == 0);
        value |= SSP2BUF;
    }
    CS = 1;
    return value;

}

void WriteDAC(char output, unsigned int value) {
    unsigned char rx;
    CS = 0;
    SSP2BUF = (output & 0x01) << 3;
    while (SSP2STATbits.BF == 0);
    rx = SSP2BUF;
    SSP2BUF = value >> 8;
    while (SSP2STATbits.BF == 0);
    rx = SSP2BUF;
    SSP2BUF = value;
    while (SSP2STATbits.BF == 0);
    rx = SSP2BUF;
    CS = 1;
}