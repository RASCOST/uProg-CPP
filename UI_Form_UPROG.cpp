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
	: TForm(Owner), avrprog(CreateProgrammer(*this))
{
	//FT_STATUS status;
	//DWORD nums;

	OpenDialogFile->Filter = "Hex files (*.hex)|*.HEX";

	//status = FT_CreateDeviceInfoList(&nums);
	//std::wstring message = L">> ";

	// check if device connected
	/*if (nums == 1) {
		message += L"Detected Device: 1";
	} else if (nums == 0) {
		message += L"Detected Device: 0! Please connect your device.";
	} else {
		message += L"Detected more than one device! Please remove your devices and try again.";
	}

	updateConsole(message.c_str());

	if (nums == 1) {
		if (avrprog->programmingEnable())
			updateConsole(L">> Device Synchronized");
	}*/
}
//---------------------------------------------------------------------------

void TForm1::updateConsole(const std::wstring& message) {
	MemoConsole->Lines->Append(message.c_str());
}

template<typename T>
void TForm1::checkBits(T *t, uint8_t *bits) {
	std::vector<TCheckBox*> checkbx;
	auto *controls =  t->Controls;

	// put all the checkboxes in a vector
	for( uint8_t idx = 0; idx < controls->Count; idx++) {
		TCheckBox *cb = dynamic_cast<TCheckBox*>(controls->Items[idx]);
		if( cb != NULL)
			checkbx.push_back(cb);
	}

	// reverse the vector because MBS is not at index 0
	std::reverse(checkbx.begin(), checkbx.end());

	// update the checkboxes
	for (auto const& e : std::as_const(checkbx)) {
		if ((*bits & 0x01) == 0) {
			e->IsChecked = true;
		} else {
			e->IsChecked = false;
		}

		*bits = *bits >> 1; // prepare to check the next bit
	}
}

void TForm1::updateLockBits(std::uint8_t *lb) {
	checkBits(groupBxLB, lb);
}

void __fastcall TForm1::BtnOpenFileClick(TObject *Sender)
{
	if (OpenDialogFile->Execute()) {
		if (FileExists(OpenDialogFile->FileName)) {
			updateConsole((L">> File: " + OpenDialogFile->FileName).c_str());
			EditFileLocation->Text = OpenDialogFile->FileName;
			avrprog->openFile(OpenDialogFile->FileName.c_str());
		} else {
            throw(Exception("File does not exists."));
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnReadLBClick(TObject *Sender)
{
	updateConsole(L">> Reading Lock Bits.");
	std::uint8_t lockBits = avrprog->readLBits();
	updateLockBits(&lockBits);
}
//---------------------------------------------------------------------------

void TForm1::updateFuseBytes(FUSE_BYTES byte, uint8_t *bits) {
	switch(byte) {
		case FUSE_BYTES::LOW:
			checkBits(PanelFLB, bits);
			break;
		case FUSE_BYTES::HIGH:
			checkBits(PanelFHB, bits);
			break;
		case FUSE_BYTES::EXTENDED:
			checkBits(PanelFEB, bits);
            break;
    }
}

void __fastcall TForm1::BtnReadFBHClick(TObject *Sender)
{
	uint8_t high = avrprog->readFsBits(FUSE_BYTES::HIGH);
	updateFuseBytes(FUSE_BYTES::HIGH, &high);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnReadFBLClick(TObject *Sender)
{
	uint8_t low = avrprog->readFsBits(FUSE_BYTES::LOW);
	updateFuseBytes(FUSE_BYTES::LOW, &low);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnReadFSEClick(TObject *Sender)
{
	uint8_t ext = avrprog->readFsBits(FUSE_BYTES::EXTENDED);
    updateFuseBytes(FUSE_BYTES::EXTENDED, &ext);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CBxDeviceChange(TObject *Sender) {
	std::wstring name = CBxDevice->Items->Strings[CBxDevice->ItemIndex].c_str();
	updateConsole(L">> Device choosed: " + name);
	avrprog->setDeviceName(name);

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

	updateConsole(message.c_str());

	// Execute command enable programming
	if (nums == 1) {
		if (avrprog->programmingEnable()) {
			updateConsole(L">> Device Synchronized");
			updateConsole(L">> Verify if selected device corresponds to connected device.");

			// check if the device connected and selected corresponds
			if(avrprog->verifySignature())
				updateConsole(L">> Device corresponds!");
			else
                updateConsole(L">> Device do not corresponds!");
		} else {
            updateConsole(L">> Device Not Synchronized");
        }
	}
}


void __fastcall TForm1::BtnWriteFlashClick(TObject *Sender)
{
    avrprog->writeFlash();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnVerifyFlashClick(TObject *Sender)
{
    avrprog->verifyFlash();
}
//---------------------------------------------------------------------------


