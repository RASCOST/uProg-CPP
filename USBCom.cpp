#include "USBCom.h"
#include "ftd2xx.h"

USBCom::USBCom()
{
}

int USBCom::getDeviceList()
{
	status = FT_CreateDeviceInfoList(&numDevices);

	if (status == FT_OK) // there is a device
	{
		return numDevices;
	}
	else
	{
		return -1;
	}
}

bool USBCom::verify(FT_STATUS status)
{
	if (status == FT_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool USBCom::openDevice()
{
	status = FT_Open(0, &ftHandle);

	return verify(status);
}

void USBCom::closeDevice()
{
	FT_Close(ftHandle);
}

bool USBCom::setFlowControl()
{
	status = FT_SetFlowControl(ftHandle, FT_FLOW_RTS_CTS, 0x11, 0x13);

	return verify(status);
}

bool USBCom::setRTS()
{
	status = FT_SetRts(ftHandle);

	return verify(status);
}

bool USBCom::clearRTS()
{
	status = FT_ClrRts(ftHandle);

	return verify(status);
}

bool USBCom::setDtr()
{
	status = FT_ClrRts(ftHandle);

	return verify(status);
}

bool USBCom::clearDtr()
{
	status = FT_ClrDtr(ftHandle);

	return verify(status);
}

int USBCom::getCTS()
{
	int recv = 0;

	status = FT_GetModemStatus(ftHandle, &cts);

	recv = 0x00000200 & cts;
	recv = recv >> 16;

	return recv;
}

int USBCom::getDRS()
{
	return 0;
}

FT_HANDLE USBCom::getHandle()
{
	return ftHandle;
}