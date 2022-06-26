//---------------------------------------------------------------------------

#ifndef HexH
#define HexH
//---------------------------------------------------------------------------

/*
* Hex file format
* : -> start in file
*
*/

class Hex {
private:
	std::uint16_t size
public:
	read(const std::wstring name);
	std::uint16_t getSize();
};
#endif
