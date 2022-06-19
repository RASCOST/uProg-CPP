#include "AVRProgrammer.h"
#include <iostream>

AVRProgrammer::AVRProgrammer(IUIProgrammer& ui) :
	device(new Device()),
	spi(new SPI()),
	state(0x08),
	ui(ui)
{
	ui.updateConsole(L">> Welcome to μProg!");
}

AVRProgrammer::~AVRProgrammer()
{
	  delete device;
	  delete spi;
}

void AVRProgrammer::init()
{
	spi->setCS(0x00); // clear the reset
	spi->clearCLK(0x00); // clear the clock
	spi->clearCS(0x08); // give a pulse reset
	Sleep(1);
	spi->setCS(0x00); // clear the reset
}

unsigned char AVRProgrammer::readInstructions(std::array<unsigned char, 3> instruction) {
    unsigned char received = 0x00;
	unsigned char data2send = 0x00; // the reset must be zero
	unsigned char temp = 0x00;

	for (int byte = 0; byte < 4; byte++)
	{
		for (int bit = 0; bit < 8; bit++)
		{
			if (byte <= 2) // Send the 3 instructions
			{
				// Start MSB
				data2send |= ((instruction[byte] << bit) & 0x80) >> 7;
				std::cout << "data: " << std::hex << +data2send << "\n";
				spi->writeMOSI(data2send);
			}

			spi->setCLK(data2send);
			Sleep(1);

			 // Read the response
			if (byte == 3)
			{
				// start MSB
				received = (received << 1) | spi->readMISO(); // shift and add the new received bit
				std::cout << "Received: " << std::hex << +temp << "\n";
			}

			spi->clearCLK(data2send);
			Sleep(1);
			data2send = data2send & 0xFE; // clear the MOSI bit of the byte to send to put new bit
		}
	}

	std::cout << "Byte Received: " << std::hex << +received << "\n";

	return received;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
bool AVRProgrammer::programmingEnable()
{
	unsigned char programEnable[] = { 0xAC, 0x53, 0x00, 0x00 };
	unsigned char received = 0x00;
	unsigned char data2send = 0x00; // the reset must be zero

	init();
	Sleep(20);

	for (int item = 0; item < 4; item++)
	{
		for (int index = 0; index < 8; index++)
		{
			// Start MSB
			data2send |= ((programEnable[item] << index) & 0x80) >> 7;
			//std::cout << "data: " << std::hex << +data2send << "\n";
			spi->writeMOSI(data2send);
			spi->setCLK(data2send);
			Sleep(1);
			if (item == 2)
			{
				// start MSB
				received = (received << 1) | spi->readMISO(); // shift and add the the new received bit
				//std::cout << "Received: " << std::hex << +received << "\n";
			}
			spi->clearCLK(data2send);
			Sleep(1);
			data2send = data2send & 0xFE; // clear the MOSI bit of the byte to send to put new bit
		}
	}
	//std::cout << "Received: " << std::hex << +received << "\n";
	if (0x53 == received) // the device is syncronized
	{
		return true;
	}
	else
	{
		return false;
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="address"></param>
/// <returns></returns>
unsigned int AVRProgrammer::readSignatureByte(unsigned int address)
{
	unsigned char readSignatureByte[] = { 0x30, 0x00, 0x00 };
	unsigned char received = 0x00;
	unsigned char data2send = 0x00; // the reset must be zero
	unsigned char temp = 0x00;
	// set the address of the device signature
	readSignatureByte[2] = readSignatureByte[2] | address;

	for (int instruction = 0; instruction < 4; instruction++)
	{
		for (int bit = 0; bit < 8; bit++)
		{
			if (instruction <= 2) // Send the 3 instructions
			{
				// Start MSB
				data2send |= ((readSignatureByte[instruction] << bit) & 0x80) >> 7;
				std::cout << "data: " << std::hex << +data2send << "\n";
				spi->writeMOSI(data2send);
			}

			spi->setCLK(data2send);
			Sleep(1);

			 // Read the response
			if (instruction == 3)
			{
				// start MSB
				received = (received << 1) | spi->readMISO(); // shift and add the the new received bit
				std::cout << "Received: " << std::hex << +temp << "\n";
			}

			spi->clearCLK(data2send);
			Sleep(1);
			data2send = data2send & 0xFE; // clear the MOSI bit of the byte to send to put new bit
		}
	}
	std::cout << "Byte Received: " << std::hex << +received << "\n";
	return received;
}

/// <summary>
/// 
/// </summary>
/// <param name="address"></param>
/// <returns></returns>
std::array<unsigned int, 3> AVRProgrammer::readSignature(unsigned int address[])
{
	std::array<unsigned int, 3> signature{};

	for (int index = 0; index < 3; index++)
	{
		signature[index] = readSignatureByte(address[index]);
	}

	return signature;
}

/// <summary>
/// 
/// </summary>
/// <param name="signature"></param>
/// <returns></returns>
bool AVRProgrammer::verifySignature(std::array<unsigned int, 3> signature)
{
	/*std::array<unsigned int, 3> deviceSignature = device->getSignatureBytes();

	for (int index = 0; index < 3; index++)
	{
		if (deviceSignature[index] != signature[index])
			return false;
	}     */

	return true;
}

void AVRProgrammer::setDeviceName(const std::wstring name) {
	  device->setDeviceName(name);
}

void AVRProgrammer::openFile(std::string fileName) {


}

void AVRProgrammer::readFsBits(FUSE_BYTES byte) {


	switch (byte) {
		case FUSE_BYTES::LOW: {
			std::array<unsigned char, 3> readFuseLowBits = { 0x50, 0x00, 0x00 };
			ui.updateConsole(L">> Reading Fuse Low Bits.");
			unsigned char fuseLowBits = readInstructions(readFuseLowBits);
			ui.updateConsole(std::to_wstring(fuseLowBits).c_str());
			break;
		}

		case FUSE_BYTES::HIGH: {
			std::array<unsigned char, 3> readFuseHighBits = { 0x58, 0x08, 0x00 };
			ui.updateConsole(L">> Reading Fuse High Bits.");
			unsigned char fuseHighBits = readInstructions(readFuseHighBits);
			ui.updateConsole(std::to_wstring(fuseHighBits).c_str());
			break;
		}

		case FUSE_BYTES::EXTENDED: {
			std::array<unsigned char, 3> readFuseExBits = { 0x50, 0x08, 0x00 };
			ui.updateConsole(L">> Reading Fuse Extended Bits.");
			unsigned char fuseExBits = readInstructions(readFuseExBits);
			ui.updateConsole(std::to_wstring(fuseExBits).c_str());
			break;
		}

		default: {
            throw "The device is not supported by μProg!";
        }
	}
}

void AVRProgrammer::writeFsBits() {

}

void AVRProgrammer::readLBits() {
	std::array<unsigned char, 3> readLockBits = { 0x58, 0x00, 0x00 };

	unsigned char lockBits = readInstructions(readLockBits);

	ui.updateConsole(std::to_wstring(lockBits).c_str());
}
void AVRProgrammer::writeLBits() {

}
void AVRProgrammer::readFlash() {

}
void AVRProgrammer::writeFlash() {

}
void AVRProgrammer::verifyFlash() {

}
void AVRProgrammer::readEeprom() {

}
void AVRProgrammer::writeEeprom() {

}
