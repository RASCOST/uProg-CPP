//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "UI_Form_UPROG.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"

TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner), avrprog(CreateProgrammer(*this))
{
	OpenDialogFile->Filter = "Hex files (*.hex)|*.HEX";
}
//---------------------------------------------------------------------------

void TForm1::updateConsole(const std::wstring& message) {
	MemoConsole->Lines->Append(message.c_str());
}

void TForm1::updateProgressBar(const float progress) {
	ProgressBar->Value = progress;
}

template<typename T>
void TForm1::checkBits(T *t, uint8_t bits) {
	uint8_t fsData = bits;
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
		if ((fsData & 0x01) == 0) {
			e->IsChecked = true;
		} else {
			e->IsChecked = false;
		}

		fsData = fsData >> 1; // prepare to check the next bit
	}
}

void TForm1::updateLockBits(const std::uint8_t lb) {
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
	avrprog->readLBits();
	//updateLockBits(&lockBits);
	/*TTask::Run(
		[this]() -> void {
			std::uint8_t lockBits = avrprog->readLBits();
			updateLockBits(lockBits);
		}

	);*/
}
//---------------------------------------------------------------------------

void TForm1::updateFuseBytes(FUSE_BYTES byte, uint8_t bits) {
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

	/*TTask::Run(
		[this, byte, bits]() -> void {
			switch(byte) {
				case FUSE_BYTES::LOW:
					TThread::Synchronize(0,
						[this, bits] () -> void {
							checkBits(PanelFLB, bits);
						}
					);
					break;
				case FUSE_BYTES::HIGH:
					TThread::Synchronize(0,
						[this, bits] () -> void {
							checkBits(PanelFHB, bits);
						}
					);
					break;
				case FUSE_BYTES::EXTENDED:
					TThread::Synchronize(0,
						[this, bits] () -> void {
							checkBits(PanelFEB, bits);
						}
					);
					break;
			}
		}
	);*/
}

void __fastcall TForm1::BtnReadFBHClick(TObject *Sender)
{
	uint8_t high = avrprog->readFsBits(FUSE_BYTES::HIGH);
	updateFuseBytes(FUSE_BYTES::HIGH, high);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnReadFBLClick(TObject *Sender)
{
	uint8_t low = avrprog->readFsBits(FUSE_BYTES::LOW);
	updateFuseBytes(FUSE_BYTES::LOW, low);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnReadFSEClick(TObject *Sender)
{
	uint8_t ext = avrprog->readFsBits(FUSE_BYTES::EXTENDED);
    updateFuseBytes(FUSE_BYTES::EXTENDED, ext);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CBxDeviceChange(TObject *Sender) {
	std::wstring name = CBxDevice->Items->Strings[CBxDevice->ItemIndex].c_str();
	updateConsole(L">> Device choosed: " + name);
	avrprog->setDeviceName(name);

	ConnectDeviceThread *connectDeviceThread = new ConnectDeviceThread(false);
}


void __fastcall TForm1::BtnWriteFlashClick(TObject *Sender)
{
	WriteFlashThread *writeFlashThread = new WriteFlashThread(false);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnVerifyFlashClick(TObject *Sender)
{
    avrprog->verifyFlash();
}
//---------------------------------------------------------------------------


