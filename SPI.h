#pragma once
#include "USB8BitMode.h"

/*********************************
 * +---------------------------+ *
 * |        SPI PIN OUT        | *
 * +---------------------------+ *
 * | FT232RL --> BYTE --> SPI  | *
 * +---------------------------+
 * |   TX    -->  D0  --> MOSI | *
 * |   RX    -->  D1  --> MISO | *
 * |   RTS   -->  D2  --> CLK  | *
 * |   CTS   -->  D3  --> CS   | *
 * +---------------------------+ *
 ********************************/

#define MOSI 0x01; //~MOSI = 0xFE
#define MISO 0x02;
#define CLK 0x04; // ~CLK = 0xFB
#define CS 0x08; // ~CS = 0xF7

class SPI
{
private:
	double halfPeriod;
	USB8BitMode* usb;
	unsigned char output;

public:
	SPI();

	void setCLK(unsigned char data);
	void clearCLK(unsigned char data);
	void writeMOSI(unsigned char data);
	int readMISO();
	void setCS(unsigned char data);
	void clearCS(unsigned char data);
	void writeByte(unsigned char data);

	unsigned char getPinMOSI();
};