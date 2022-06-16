#pragma once
#include <array>

#include "SPI.h"
#include "Device.h"
#include "IProgrammer.h"

class AVRProgrammer : public IProgrammer
{
private:
	SPI* spi;
	Device* device;
	IUIProgrammer& ui;
	unsigned char state;


public:
	AVRProgrammer(IUIProgrammer& ui);
	~AVRProgrammer();

	// methods
	bool programmingEnable();
	std::array<unsigned int, 3> readSignature(unsigned int address[]);
	bool verifySignature(std::array<unsigned int, 3> signature);

	void openFile(std::string fileName);
	void readFsBits(FUSE_BYTES byte);
	void writeFsBits();
	void readLBits();
	void writeLBits();
	void readFlash();
	void writeFlash();
	void verifyFlash();
	void readEeprom();
	void writeEeprom();

private:
	void init();
	unsigned int readSignatureByte(unsigned int address);
	unsigned char readInstructions(std::array<unsigned char, 3> instruction);
};