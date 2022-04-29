#pragma once
#include "SPI.h"
#include "Device.h"
#include <array>

class AVRProgrammer
{
private:
	SPI* spi;
	unsigned char state;

public:
	AVRProgrammer();
	~AVRProgrammer();
	Device* device;
	// methods
	bool programmingEnable();
	std::array<unsigned int, 3> readSignature(unsigned int address[]);
	bool verifySignature(std::array<unsigned int, 3> signature);

private:
	void init();
	unsigned int readSignatureByte(unsigned int address);
};