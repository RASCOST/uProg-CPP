//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "UI_Form_UPROG.h"
#include "ftd2xx.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	FT_STATUS status;
	DWORD nums;

	status = FT_CreateDeviceInfoList(&nums);
}

//---------------------------------------------------------------------------

