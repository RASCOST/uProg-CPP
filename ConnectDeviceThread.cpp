//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "ConnectDeviceThread.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall ConnectDeviceThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall ConnectDeviceThread::ConnectDeviceThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall ConnectDeviceThread::Execute()
{
	NameThreadForDebugging(System::String(L"ConnectDeviceThread"));
	//---- Place thread code here ----

	FreeOnTerminate = true;
    // check if there is a device connected
    FT_STATUS status;
	DWORD nums;

    status = FT_CreateDeviceInfoList(&nums);
	std::wstring message = L">> ";

    if (nums == 1) {
		message += L"Detected Device: 1";
	} else if (nums == 0) {
		message += L"Detected Device: 0! Please connect your device.";
	} else {
		message += L"Detected more than one device! Please remove your devices and try again.";
	}
	Synchronize([message] {
		Form1->updateConsole(message);
	});

	// Execute command enable programming
	if (nums == 1) {
		if (Form1->avrprog->programmingEnable()) {
			Synchronize([] {
				Form1->updateConsole(L">> Device Synchronized... \n Verifying if the selected device corresponds to the connected device...");
			});
			// check if the device connected and selected corresponds
			if(Form1->avrprog->verifySignature())
				Synchronize([] {
					Form1->updateConsole(L">> Device corresponds!");
				});
			else
				Synchronize([message] {
					Form1->updateConsole(L">> Device do not corresponds!");
				});
		} else {
            Synchronize([] {
				Form1->updateConsole(L">> Device Not Synchronized");
			});
        }
	}
}
//---------------------------------------------------------------------------
