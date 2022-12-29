//---------------------------------------------------------------------------

#ifndef WriteFlashThreadH
#define WriteFlashThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>

#include "UI_Form_UPROG.h"
//---------------------------------------------------------------------------
class WriteFlashThread : public TThread
{
protected:
	void __fastcall Execute();
public:
	__fastcall WriteFlashThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
