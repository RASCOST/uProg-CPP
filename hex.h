//---------------------------------------------------------------------------

#ifndef HexH
#define HexH
//---------------------------------------------------------------------------
#include <string>
#include <fstream>

/*
* Hex file format -> :llaaaatt[dd...]cc
* : -> start of line in file
* ll -> number of data bytes (dd)
* aaaa -> start address for data in the line
* tt ->  type of record
*    -> 00 - data record
*    -> 01 - end of file record
*    -> 02 - extended segment address record
*    -> 04 - extended linear adddress record
*    -> 05 - start linear address
* cc -> checksum
*/

class Hex {
private:
	std::uint16_t size;
	bool eof(const std::string line);

public:
	Hex();
	void read(const std::wstring name);
	std::uint16_t getSize();
};
#endif
