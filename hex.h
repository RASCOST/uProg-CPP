//---------------------------------------------------------------------------

#ifndef HexH
#define HexH
//---------------------------------------------------------------------------
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

/*
* Hex file format -> :llaaaatt[dd...]cc
* :    -> start of line in file
* ll   -> number of data bytes (dd)
* aaaa -> start address for data in the line
* tt   -> type of record
*        -> 00 - data record
*        -> 01 - end of file record
*        -> 02 - extended segment address record
*        -> 04 - extended linear adddress record
*        -> 05 - start linear address
* cc   -> checksum
*/

class Hex {
private:
	std::uint16_t size;
	std::vector<uint8_t> data;

	bool eof(const std::string line);
	std::uint16_t dataLength(const std::string line);
	void readData(const std::string line);

public:
	Hex();
	void read(const std::wstring name);
	std::uint16_t getSize();
	std::vector<uint8_t> getData();
};
#endif
