//---------------------------------------------------------------------------

#ifndef ConnectDeviceThreadH
#define ConnectDeviceThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>

#include "UI_Form_UPROG.h"
//---------------------------------------------------------------------------
class ConnectDeviceThread : public TThread
{
protected:
	void __fastcall Execute();
public:
	__fastcall ConnectDeviceThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
