#pragma once
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <System.Threading.hpp>

#include <array>
#include <chrono>

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
	void readLBits();
	void writeLBits();
	void readFlash();
	void writeFlash(TThread* thread);
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
	void delay_ms(uint16_t ms);
};