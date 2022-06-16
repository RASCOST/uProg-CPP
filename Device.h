#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <System.JSON.hpp>
#include <System.Classes.hpp>

struct FuseBytes {
private:
	std::vector<std::wstring> extended;
	std::array<std::wstring, 8> high;
	std::array<std::wstring, 8> low;

public:
	void setExtended(TJSONArray* array);
	void setHigh(TJSONArray* array);
	void setLow(TJSONArray* array);
};

struct Flash {

};

struct EEPROM {

};

struct Architecture {
	std::unique_ptr<FuseBytes> fuseBytes = std::make_unique<FuseBytes>();
	std::unique_ptr<Flash> flash = std::make_unique<Flash>();;
	std::unique_ptr<EEPROM> eeprom = std::make_unique<EEPROM>();;
	std::vector<std::wstring> LockBits;
	std::array<std::uint8_t, 3> Signature_Bytes_Address;
	std::array<std::uint8_t, 3> Signature_Bytes;
	std::uint8_t Flash_Size;
	std::uint8_t Flash_Page_Size;
	std::uint8_t Flash_PCWORD;
	std::uint8_t Flash_Number_of_Pages;
	std::uint8_t Flash_PCPAGE;
	std::uint8_t EEPROM_Size;
	std::uint8_t EEPROM_Page_size;
	std::uint8_t EEPROM_PCWORD;
	std::uint8_t EEPROM_PCPAGE;
	std::uint8_t EEPROM_EEAMSB;

	void setLockBits(TJSONArray* lockBits);
	void setSignatureBytesAddress(TJSONArray* address);
	void setSignatureBytes(TJSONArray* signature);

};

class Device {
private:
	std::wstring name;
	std::unique_ptr<Architecture> architecture;

	void readJSONFile();

public:
	Device(const std::wstring name);
	void setDeviceName(const std::wstring device);
	const std::wstring getDeviceName();
};
