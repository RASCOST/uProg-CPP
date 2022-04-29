#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <array>

struct Memory {
	std::array<unsigned int, 3> Signature_Bytes_Address;
	std::array<unsigned int, 3> Signature_Bytes;
	unsigned int Flash_Size;
	unsigned int Flash_Page_Size;
	unsigned int Flash_PCWORD;
	unsigned int Flash_Number_of_Pages;
	unsigned int Flash_PCPAGE;
	unsigned int Flash_PCMSB;
	unsigned intEEPROM_Size;
	unsigned intEEPROM_Page_size;
	unsigned intEEPROM_PCWORD;
	unsigned intEEPROM_Number_of_Pages;
	unsigned intEEPROM_PCPAGE;
	unsigned intEEPROM_EEAMSB;
};

class Device
{
private:
	std::string path;
	std::string name;
	

	Memory* memory;

	void setName(std::string name);
	unsigned int searchPosition(std::string str, char c);

public:
	Device(std::string name);
	std::string readFile(std::string path, std::string strToSearch);
	void setPath(std::string path);
	void setSignatureBytesAddress();
	void setSignatureBytes();
	void setFlashSize();

	//Tests
	std::string getDeviceName();
	std::array<unsigned int, 3> getSignatureBytesAddress();
	std::array<unsigned int, 3> getSignatureBytes();
	unsigned int getFlashSize();
};