#include "AVRProgrammer.h"
#include <iostream>

AVRProgrammer::AVRProgrammer(IUIProgrammer& ui) :
	device(new Device()),
	spi(new SPI()),
	hex(new Hex()),
	state(0x08),
	ui(ui)
{
	ui.updateConsole(L">> Welcome to μProg!");
}

AVRProgrammer::~AVRProgrammer()
{
	  delete device;
	  delete spi;
      delete hex;
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

void AVRProgrammer::writeInstructions(std::array<uint8_t, 4> instruction) {
    uint8_t data2send = 0x00; // the reset must be zero

	for (uint8_t byte = 0; byte < 4; byte++)
	{
		for (int bit = 0; bit < 8; bit++)
		{
			// Start MSB
			data2send |= ((instruction[byte] << bit) & 0x80) >> 7;
			std::cout << "data: " << std::hex << +data2send << "\n";
			spi->writeMOSI(data2send);

			spi->setCLK(data2send);
			Sleep(1);
			spi->clearCLK(data2send);
			Sleep(1);
			data2send = data2send & 0xFE; // clear the MOSI bit of the byte to send to put new bit
		}
	}
}

bool AVRProgrammer::polling() {
	std::array<uint8_t, 3> poll = {0xF0,0x00, 0x00};

	uint8_t recv = readInstructions(poll);

	if (recv != 0xFE) // if busy
		return false;

	return true;
}

void AVRProgrammer::chipErase() {
	std::array<uint8_t, 4> erase = {0xAC,0x80, 0x00, 0x00};

	writeInstructions(erase);
	Sleep(20);
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
unsigned char AVRProgrammer::readSignatureByte(unsigned char address)
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
std::array<unsigned char, 3> AVRProgrammer::readSignature(std::array<unsigned char, 3> address)
{
	std::array<unsigned char, 3> signature{};

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
bool AVRProgrammer::verifySignature()
{
	std::array<unsigned char, 3> signature = device->getSignatureBytes();
	std::array<unsigned char, 3> deviceSignature = readSignature(device->getSignatureBytesAddress());

	for (int index = 0; index < 3; index++)
	{
		if (deviceSignature[index] != signature[index])
			return false;
	}

	return true;
}

void AVRProgrammer::setDeviceName(const std::wstring name) {
	  device->setDeviceName(name);
}

void AVRProgrammer::openFile(std::wstring fileName) {
	hex->read(fileName);
    ui.updateConsole(L">> File Size: " + std::to_wstring(hex->getSize()) + L"Bytes");
}

uint8_t AVRProgrammer::readFsBits(FUSE_BYTES byte) {
	uint8_t bits = 0;

	switch (byte) {
		case FUSE_BYTES::LOW: {
			std::array<unsigned char, 3> readFuseLowBits = { 0x50, 0x00, 0x00 };
			ui.updateConsole(L">> Reading Fuse Low Bits.");
			bits = readInstructions(readFuseLowBits);
			ui.updateConsole(std::to_wstring(bits));
			break;
		}

		case FUSE_BYTES::HIGH: {
			std::array<unsigned char, 3> readFuseHighBits = { 0x58, 0x08, 0x00 };
			ui.updateConsole(L">> Reading Fuse High Bits.");
			bits = readInstructions(readFuseHighBits);
			ui.updateConsole(std::to_wstring(bits));
			break;
		}

		case FUSE_BYTES::EXTENDED: {
			std::array<unsigned char, 3> readFuseExBits = { 0x50, 0x08, 0x00 };
			ui.updateConsole(L">> Reading Fuse Extended Bits.");
			bits = readInstructions(readFuseExBits);
			ui.updateConsole(std::to_wstring(bits));
			break;
		}

		default: {
            throw "The device is not supported by μProg!";
		}
	}

	return bits;
}

void AVRProgrammer::writeFsBits() {

}

std::uint8_t AVRProgrammer::readLBits() {
	std::array<unsigned char, 3> readLockBits = { 0x58, 0x00, 0x00 };

	std::uint8_t lockBits = readInstructions(readLockBits);

	ui.updateConsole(std::to_wstring(lockBits).c_str());

	return lockBits;
}

void AVRProgrammer::writeLBits() {

}
void AVRProgrammer::readFlash() {

}

void AVRProgrammer::loadMemoryPage(std::array<uint8_t, 4> instruction) {
	writeInstructions(instruction);
}

void AVRProgrammer::writeMemoryPage(std::array<uint8_t, 4> instruction) {
	writeInstructions(instruction);
}

void AVRProgrammer::writeFlash() {
	std::array<uint8_t, 4> memoryPage = {0x4C, 0x00, 0x00, 0x00};
	std::array<uint8_t, 4> lowByte = {0x40, 0x00, 0x00, 0x00};
	std::array<uint8_t, 4> highByte = {0x48, 0x00, 0x00, 0x00};
	std::vector<uint8_t> data = hex->getData();
	uint16_t pcword = 0;
	uint16_t pcpage = 0;
	uint16_t address = 0;
	uint32_t idx = 0;
	uint32_t dataSize = hex->getSize();
	bool FLAG_PAGE_PROGRAMMED = false;

	// before write in flash an erase must be done
	ui.updateConsole(L">> Erasing device...");
	chipErase();
	ui.updateConsole(L">> Device erased...");

	while ( idx < dataSize) {
		if (pcword < device->getPageSize()) {
			// prepare the address
			lowByte[2] = pcword;
			// prepare the data
			lowByte[3] = data[idx];

			loadMemoryPage(lowByte);

			/*std::wstring hex_low = IntToHex(data[idx+1]).c_str();
			std::wstring hex_high = IntToHex(data[idx]).c_str();
			ui.updateConsole(L">>"  + hex_high + hex_low); */

			// prepare the address
			//highByte[1] = pcpage;
			highByte[2] = pcword;
			// prepare the data
			highByte[3] = data[idx+1];

			loadMemoryPage(highByte);

			// update the address of the buffer
			pcword++;
			idx += 2;
			FLAG_PAGE_PROGRAMMED = false;
			ui.updateConsole(L">> Percentage: " + std::to_wstring((idx/dataSize)*100.0));
			ui.updateProgressBar(static_cast<float>(idx/dataSize)*100.0);
		} else {
			if (pcpage < device->getNumberPages()) {
				ui.updateConsole(L">> Writing page: " + std::to_wstring(pcpage));
				address = static_cast<uint16_t>(pcpage << 5);

				/*std::wstring hex_address = IntToHex(address).c_str();
				ui.updateConsole(L">> Page Address: " + hex_address);

				memoryPage[1] = static_cast<uint8_t>(address >> 8);
				memoryPage[2] = static_cast<uint8_t>(address);

				std::wstring low_address = IntToHex(static_cast<uint8_t>(address)).c_str();
				std::wstring high_address = IntToHex(static_cast<uint8_t>(address >> 8)).c_str();
				ui.updateConsole(L">> Page Address low: " + low_address);
				ui.updateConsole(L">> Page Address high: " + high_address); */

				pcword = 0;

				// store page
				writeMemoryPage(memoryPage);

				// wait until ready
				while(!polling());

				// update the address of the program memory
				pcpage++;
				FLAG_PAGE_PROGRAMMED = true;
			}
		}
	}

	if (!FLAG_PAGE_PROGRAMMED) {
		ui.updateConsole(L">> Writing page: " + std::to_wstring(pcpage));

		address = static_cast<uint16_t>(pcpage << 5);

		/*std::wstring hex_address = IntToHex(address).c_str();
		ui.updateConsole(L">> Page Address: " + hex_address);*/

		memoryPage[1] = static_cast<uint8_t>(address >> 8);
		memoryPage[2] = static_cast<uint8_t>(address);

		/*std::wstring low_address = IntToHex(static_cast<uint8_t>(address)).c_str();
		std::wstring high_address = IntToHex(static_cast<uint8_t>(address >> 8)).c_str();
		ui.updateConsole(L">> Page Address low: " + low_address);
		ui.updateConsole(L">> Page Address high: " + high_address); */

		// store page
		writeMemoryPage(memoryPage);

        // wait until ready
		while(!polling());
	}

	//verifyFlash();
}

void AVRProgrammer::verifyFlash() {
	std::array<uint8_t, 3> lowByte = {0x20, 0x00, 0x00};
	std::array<uint8_t, 3> highByte = {0x28, 0x00, 0x00};
    uint16_t pcword = 0;
	uint16_t pcpage = 0;
	std::vector<uint8_t> program;
	std::vector<uint8_t> file = hex->getData();
	uint8_t low, high;
	uint16_t address;

	ui.updateConsole(L">> Verifying...");

	while(true) {

		address = pcpage << 5;

		highByte[1] = static_cast<uint8_t> (address >> 8);
		highByte[2] = static_cast<uint8_t> (address);
		high = readInstructions(highByte);


		lowByte[1] = static_cast<uint8_t> (address >> 8);
		lowByte[2] = static_cast<uint8_t> (address);
		low = readInstructions(lowByte);
		/*std::wstring hex_low = IntToHex(low).c_str();
		std::wstring hex_high = IntToHex(high).c_str();
		ui.updateConsole(L">>Before compare: "  + hex_high + hex_low); */
        if (low == 0xFF && high == 0xFF)
			break;

		program.push_back(high);
		program.push_back(low);

		if (pcword == 31) {
			pcword = 0;
			pcpage++;
		} else {
            pcword++;
		}
	}

	for (uint16_t idx = 0; idx < file.size(); idx++) {
		/*std::wstring f = IntToHex(file[idx]).c_str(); //std::wstring(IntToHex(file[idx]).begin(), IntToHex(file[idx]).end());
		std::wstring p = IntToHex(program[idx]).c_str(); //std::wstring(IntToHex(program[idx]).begin(), IntToHex(program[idx]).end());
		ui.updateConsole(L"file: " +  f
			+
			L" -- memory: " + p
			); */
		if (file[idx] != program[idx]) {
			ui.updateConsole(L">> Memory and file do not correspond!");
			break;
		}
	}
	/*ui.updateConsole(L">> file size: " + std::to_wstring(file.size()) + L" memory size: " + std::to_wstring(program.size()));*/
    ui.updateConsole(L">> Verifying Finished!");
}

void AVRProgrammer::readEeprom() {

}
void AVRProgrammer::writeEeprom() {

}
