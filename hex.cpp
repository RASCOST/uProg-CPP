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

uint16_t Hex::getSize() {
	return (uint16_t)data.size();
}

 std::vector<uint8_t> Hex::getData() {
     return data;
 }

std::uint16_t Hex::dataLength(const std::string line) {
	return std::stoi(line.substr(1,2), 0, 16);
}

void Hex::readData(const std::string line) {
	std::string bytes = line.substr(9,(dataLength(line)*2));
	uint16_t size = dataLength(line);

	for (uint16_t counter = 0; counter < size; counter+=2) {
		data.push_back(std::stoi(bytes.substr(counter, 2), 0, 16));
	}
}

void Hex::read(const std::wstring name) {
	std::fstream file;
	std::string line;
	std:uint16_t lineSize = 0;

	// open file
	file.open(name);
	try {
        if (!file.is_open())
		throw(std::exception("File does not exists."));
    }
	catch (std::exception& e) {
		  std::cout << e.what() << "\n";
	}

	// read lines
	while(getline(file,line)) {
	   if (eof(line))
		break;
	   readData(line);
    }
}
