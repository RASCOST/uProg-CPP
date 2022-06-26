//---------------------------------------------------------------------------

#pragma hdrstop

#include "Hex.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

Hex::Hex() : size(0) {};

bool Hex::eof(const std::string line) {
	if (line.substr(7,2) == "01")
		return true;

	return false;
}

std::uint16_t Hex::getSize(const std::string line) {
	return std::stoi(line.substr(1,2));
}

void Hex::read(const std::wstring name) {
	std::fstream file;
	std::string line;
	std:uint16_t lineSize = 0;

	// open file
	file.open(name);
	if (!file.is_open())
		throw(Exception("File does not exists."));

	// read lines
	while(getline(file,line)) {
	   if (eof(line))
		break;


    }
}
