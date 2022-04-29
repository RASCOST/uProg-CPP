#pragma once
#include "ftd2xx.h"

class USBCom
{
	public:
		// constructor
		USBCom();
		// destructor
		//~USBCom();

		// methods
		int getDeviceList(); // verify is there is one device
		bool openDevice();
		void closeDevice();
		bool setFlowControl();
		bool setRTS();
		bool clearRTS();
		bool setDtr();
		bool clearDtr();
		int getCTS();
		int getDRS();
		FT_HANDLE getHandle();

	private:
		// attributes
		FT_HANDLE ftHandle;
		FT_STATUS status;
		DWORD numDevices;
		DWORD cts;
		DWORD drs;

		bool verify(FT_STATUS status);
};