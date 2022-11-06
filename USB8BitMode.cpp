#include "USB8BitMode.h"
#include<iostream>
#include<iomanip>

/******************************
 * +------------------------+ *
 * |    FT232RL PIN OUT     | *
 * +------------------------+ *
 * | Pin 6  ( RI# )  --> D7 | *
 * | Pin 10 ( DCD# ) --> D6 | *
 * | Pin 9  ( DSR# ) --> D5 | *
 * | Pin 2  ( DTR# ) --> D4 | *
 * | Pin 11 ( CTS# ) --> D3 | *
 * | Pin 3  ( RTS# ) --> D2 | *
 * | Pin 5  ( RXD )  --> D1 | *
 * | Pin 1  ( TXD )  --> D0 | *
 * +------------------------+ *
 * ***************************/

 /**************************************
  * +--------------------------------+ *
  * |    FT232RL PIN OUT             | *
  * +--------------------------------+ *
  * | Pin 6  ( RI# )  --> D7         | *
  * | Pin 10 ( DCD# ) --> D6         | *
  * | Pin 9  ( DSR# ) --> D5         | *
  * | Pin 2  ( DTR# ) --> D4         | *
  * | Pin 11 ( CTS# ) --> Output (1) | *
  * | Pin 3  ( RTS# ) --> Output (1) | *
  * | Pin 5  ( RXD )  --> Input  (0) | *
  * | Pin 1  ( TXD )  --> Output (1) | *
  * +--------------------------------+ *
  * ***********************************/

/// <summary>
/// 
/// </summary>
USB8BitMode::USB8BitMode()
{
	if (getDeviceList() == -1)
	{
		std::cout << "No device connected!" << "\n";
		return;
	}
	else if(openDevice())
	{
		std::cout << "Device opened!" << "\n";
	}
	else
	{
		std::cout << "Error opening device!" << "\n";
		closeDevice();
		return;
	}

	FT_SetFlowControl(getHandle(), FT_FLOW_NONE, 0x11, 0x13);
	status = FT_SetBaudRate(getHandle(), FT_BAUD_9600);
	std::cout << "Set baudrate: " << status << "\n";
	status = FT_SetBitMode(getHandle(), IO_MASK, IO_MODE);
	std::cout << "Set bitmode: " << status << "\n";
}

/// <summary>
/// 
/// </summary>
USB8BitMode::~USB8BitMode()
{
	closeDevice();
}

/// <summary>
/// 
/// </summary>
/// <param name="byte"></param>
void USB8BitMode::writeBitMode(UCHAR data)
{
	DWORD bytesTX = 0;

	//std::cout << "data: " << data << " length: " << sizeof(data) << "\n";
	status = FT_Write(getHandle(), &data, sizeof(data), &bytesTX);
	//std::cout << "tx data: " << status << " bytes tx: " << bytesTX << "\n";
}

int USB8BitMode::readBitMode()
{
	UCHAR readed;

	status = FT_GetBitMode(getHandle(), &readed);

	//std::cout << "data: " << std::hex << std::uppercase << "0x" << +readed << " status: " << status << "\n";
	return (int)readed;
}
