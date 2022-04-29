#pragma once
#include "ftd2xx.h"
#include "USBCom.h"

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
  * | Pin 3  ( RTS# ) --> Outpu  (1) | *
  * | Pin 5  ( RXD )  --> Input  (0) | *
  * | Pin 1  ( TXD )  --> Output (1) | *
  * +--------------------------------+ *
  * ***********************************/

class USB8BitMode: public USBCom
{
public:
	USB8BitMode();
	~USB8BitMode();

	void writeBitMode(UCHAR data);
	int readBitMode();

private:
	//FT_HANDLE ftHandle;
	FT_STATUS status;
	UCHAR IO_MASK = 0x0D;
	UCHAR IO_MODE = 1;
};