#include "Device.h"

/// <summary>
/// 
/// </summary>
Device::Device(std::string name)
{
	setName(name);
	memory = new Memory();
}

/// <summary>
/// 
/// </summary>
/// <param name="path"></param>
void Device::setPath(std::string path)
{
	this->path = path;
}

/// <summary>
/// 
/// </summary>
/// <param name="name"></param>
void Device::setName(std::string name)
{
	this->name = name;
}

/// <summary>
/// 
/// </summary>
/// <param name="path"></param>
/// <param name="strToSearch"></param>
/// <returns></returns>
std::string Device::readFile(std::string path, std::string strToSearch)
{
	std::string line;
	std::ifstream file(path);
	unsigned int charPos = 0;

	//std::cout << strToSearch << "\n";
	while (!file.eof())
	{
		getline(file, line);

		if (line.find(strToSearch) != std::string::npos)
		{
			std::cout << line << '\n';
			break;
		}
	}

	file.close();

	return line;
}

/// <summary>
/// 
/// </summary>
/// <param name="str"></param>
/// <param name="c"></param>
/// <returns></returns>
unsigned int Device::searchPosition(std::string str, char c)
{
	return str.find(c) + 1;
}

/// <summary>
/// 
/// </summary>
void Device::setSignatureBytesAddress()
{
	std::string str = readFile(this->path, "Signature_Address");
	unsigned int position = searchPosition(str, '=');
	std::string number = "";
	std::cout << str.at(position) << "\n";
	for (int index = 0; index < 3; index++)
	{
		
		while (str.at(position) != ']')
		{
			
			if ((str.at(position) != ',') && (str.at(position) == '['))
			{
				std::cout << str.at(position) << "\n";
				number += str.at(position);
				position++;
			}
			else
			{
				position++;
				break;
			}
		}
		//std::cout << std::stoi(number) << "\n";
		this->memory->Signature_Bytes_Address[index] = std::stoi(number, 0, 16);
		number = "";
	}
}

/// <summary>
/// 
/// </summary>
void Device::setSignatureBytes()
{
	std::string str = readFile(this->path, "Signature_Bytes");
	//std::cout << str << '\n';
	unsigned int position = searchPosition(str, '=');
	std::string number = "";

	for (int index = 0; index < 3; index++)
	{
		while (str.at(position) != ']')
		{
			if (str.at(position) != ',')
			{
				number += str.at(position);
				position++;
			}
			else
			{
				position++;
				break;
			}
		}
		//std::cout << std::stoi(number) << "\n";
		this->memory->Signature_Bytes[index] = std::stoi(number, 0, 16);
		number = "";
	}
}

/// <summary>
/// 
/// </summary>
void Device::setFlashSize()
{
	std::string str = readFile(this->path, "Flash_Size");
	//std::cout << str << '\n';
	unsigned int position = searchPosition(str, '=');
	std::string number = std::string(1,str.at(position));
	this->memory->Flash_Size = std::stoi(number);
}

// Tests
std::string Device::getDeviceName()
{
	return this->name;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
std::array<unsigned int, 3> Device::getSignatureBytesAddress()
{
	return this->memory->Signature_Bytes_Address;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
std::array<unsigned int, 3> Device::getSignatureBytes()
{
	return this->memory->Signature_Bytes;
}

unsigned int Device::getFlashSize()
{
	return this->memory->Flash_Size;
}