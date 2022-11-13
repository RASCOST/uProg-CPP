#pragma once
#include <System.SysUtils.hpp>
#include <array>
#include <System.Classes.hpp>

#include "SPI.h"
#include "Device.h"
#include "IProgrammer.h"
#include "Hex.h"

class AVRProgrammer : public IProgrammer
{
private:
	SPI* spi;
	Device* device;
	Hex* hex;
	IUIProgrammer& ui;
	unsigned char state;


public:
	AVRProgrammer(IUIProgrammer& ui);
	~AVRProgrammer();

	// methods
	bool programmingEnable();
	std::array<unsigned char, 3> readSignature(std::array<unsigned char, 3>  address);

	bool verifySignature();
    void setDeviceName(const std::wstring name);
	void openFile(std::wstring fileName);
	uint8_t readFsBits(FUSE_BYTES byte);
	void writeFsBits();
	std::uint8_t readLBits();
	void writeLBits();
	void readFlash();
	void writeFlash();
	void verifyFlash();
	void readEeprom();
	void writeEeprom();

private:
	void init();
	unsigned char readSignatureByte(unsigned char address);
	unsigned char readInstructions(std::array<unsigned char, 3> instruction);
	void writeInstructions(std::array<uint8_t, 4> instruction);
	void loadMemoryPage(std::array<uint8_t, 4> instruction);
	void writeMemoryPage(std::array<uint8_t, 4> instruction);
	bool polling();
    void chipErase();
};