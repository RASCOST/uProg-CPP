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
private:
	std::wstring flashSize;
	std::uint16_t pageSize;
	std::vector<std::uint8_t> pcWord;
	std::uint16_t numberPages;
	std::vector<std::uint8_t> pcPage;
	std::uint16_t pcMsb;

public:
	void setFlashSize(std::wstring flashsize);
	void setPageSize(std::wstring pagesize);
	void setPcWord(TJSONArray* array);
	void setNumberPages(std::uint16_t pages);
	void setPcPage(TJSONArray* array);
	void setPcMsb(std::uint16_t pcmsb);
	uint16_t getNumberPages();
	uint16_t getPageSize();
};

struct EEPROM {
private:
	std::wstring eepromSize;
	std::wstring pageSize;
	std::vector<std::uint8_t> pcWord;
	std::uint16_t numberPages;
	std::vector<std::uint8_t> pcPage;
	std::uint16_t pcMsb;

public:
	void setEepromSize(std::wstring eepromsize);
	void setPageSize(std::wstring pagesize);
	void setPcWord(TJSONArray* array);
	void setNumberPages(std::uint16_t pages);
	void setPcPage(TJSONArray* array);
	void setPcMsb(std::uint16_t pcmsb);
};

struct Architecture {
	std::unique_ptr<FuseBytes> fuseBytes = std::make_unique<FuseBytes>();
	std::unique_ptr<Flash> flash = std::make_unique<Flash>();;
	std::unique_ptr<EEPROM> eeprom = std::make_unique<EEPROM>();;
	std::vector<std::wstring> LockBits;
	std::array<std::uint8_t, 3> Signature_Bytes_Address;
	std::array<std::uint8_t, 3> Signature_Bytes;

	void setLockBits(TJSONArray* lockBits);
	void setSignatureBytesAddress(TJSONArray* address);
	void setSignatureBytes(TJSONArray* signature);
};

class Device {
private:
	std::wstring name;
	std::unique_ptr<Architecture> architecture;

	void setFuseBytes(TJSONObject* object);
	void setFlash(TJSONObject* object);
	void setEEPROM(TJSONObject* object);
	void readJSONFile();

public:
	Device();
	void setDeviceName(const std::wstring device);
	const std::wstring getDeviceName();
    std::array<std::uint8_t, 3> getSignatureBytesAddress();
	std::array<std::uint8_t, 3> getSignatureBytes();
	uint16_t getPageSize();
};
