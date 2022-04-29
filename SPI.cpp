#include "SPI.h"

SPI::SPI()
{
	this->output = 0;
	this->usb = new USB8BitMode();
}

void SPI::setCLK(unsigned char data)
{
	// clear the the bit of CLK -> (data & 0xFB)
	// set the new value to the CLK pin -> | CLK
	unsigned char dataToSend = (data & 0xFB) | CLK;
	usb->writeBitMode(dataToSend);
}

void SPI::clearCLK(unsigned char data)
{
	// clear the the bit of CLK -> (data & 0xFB)
	unsigned char dataToSend = (data & 0xFB);
	usb->writeBitMode(dataToSend);
}

void SPI::writeMOSI(unsigned char data)
{
	usb->writeBitMode(data);
}

int SPI::readMISO()
{
	// clear all the other bits in the pins
	int received = usb->readBitMode() & MISO;

	// shift by 1 bit the readed value
	return received >> 1;
}

void SPI::setCS(unsigned char data)
{
	// clear the the bit of CS -> (data & 0xF7)
	// set the new value to the CS pin -> | CS
	unsigned char dataToSend = (data & 0xF7) | CS;
	usb->writeBitMode(dataToSend);
}

void SPI::clearCS(unsigned char data)
{
	// clear the the bit of CLK -> (data & 0xF7)
	unsigned char dataToSend = (data & 0xF7);
	usb->writeBitMode(dataToSend);
}

void SPI::writeByte(unsigned char data)
{
	unsigned char dataToSend = data;
	unsigned char bit = data & MOSI;

	for (unsigned int index = 0; index < 8; index++)
	{
		// set data in the MOSI Pin;
		writeMOSI(bit);
		// set the clock
		setCLK(bit);
		// wait for 1ms
		Sleep(1);
		clearCLK(bit);
		// set new bit to be send
		dataToSend = dataToSend >> 1;
		bit = dataToSend & 0x01;
		Sleep(1);
	}
}

unsigned char SPI::getPinMOSI()
{
	return MOSI;
}