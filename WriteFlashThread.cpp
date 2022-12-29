//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "WriteFlashThread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall WriteFlashThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall WriteFlashThread::WriteFlashThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall WriteFlashThread::Execute()
{
	NameThreadForDebugging(System::String(L"WriteFlashThread"));
	//---- Place thread code here ----

	FreeOnTerminate = true;

	std::array<uint8_t, 4> memoryPage = {0x4C, 0x00, 0x00, 0x00};
	std::array<uint8_t, 4> lowByte = {0x40, 0x00, 0x00, 0x00};
	std::array<uint8_t, 4> highByte = {0x48, 0x00, 0x00, 0x00};
	std::vector<uint8_t> data = Form1->avrprog->hex->getData();
	uint32_t dataSize = hex->getSize();
	uint32_t idx = 0;
	uint16_t pcword = 0;
	uint16_t pcpage = 0;
	uint16_t address = 0;

	bool FLAG_PAGE_PROGRAMMED = false;

	// before write in flash an erase must be done
	//ui.updateConsole(L">> Erasing device...");
	Synchronize([] {
		Form1->updateConsole(L">> Erasing device...");
	});
	chipErase();
	//ui.updateConsole(L">> Device erased...");
	//ui.updateConsole(L">> Start programming device...");
	Synchronize([] {
		Form1->updateConsole(L">> Device erased... \n >> Start programming device...");
	});

	while ( idx < dataSize) {
		if (pcword < device->getPageSize()) {
			// prepare the address
			lowByte[2] = pcword;
			// prepare the data
			lowByte[3] = data[idx];

			Form1->avrprog->loadMemoryPage(lowByte);

			// prepare the address
			highByte[2] = pcword;
			// prepare the data
			highByte[3] = data[idx+1];

			Form1->avrprog->loadMemoryPage(highByte);

			// update the address of the buffer
			pcword++;
			idx += 2;
			FLAG_PAGE_PROGRAMMED = false;
			//std::wstring hex_low = IntToHex(data[idx]).c_str();
			//std::wstring hex_high = IntToHex(data[idx+1]).c_str();
			//ui.updateConsole(L">> Data " + std::to_wstring(idx) + L" : " + hex_high + hex_low);
			Synchronize([idx, device->getPageSize()] {
				Form1->updateProgressBar(static_cast<float>(idx/device->getPageSize())*100.0);
			});
		} else {
		// TRY
			if (pcpage < device->getNumberPages()) {
				//ui.updateProgressBar(static_cast<float>(idx/device->getPageSize())*100.0);
				Synchronize([pcpage] {
					Form1->updateConsole(L">> Writing page: " + std::to_wstring(pcpage));
				});
				//ui.updateConsole(L">> Writing page: " + std::to_wstring(pcpage));

				address = static_cast<uint16_t>(pcpage << 5);
				memoryPage[1] = static_cast<uint8_t>(address >> 8);
				memoryPage[2] = static_cast<uint8_t>(address);

				// reset the address of the page buffer
				pcword = 0;

				// store page
				Form1->avrprog->writeMemoryPage(memoryPage);

				// wait until ready
				while(!Form1->avrprog->polling());

				// update the address of the program memory
				pcpage++;
				FLAG_PAGE_PROGRAMMED = true;
			}
		}
	}

	if (!FLAG_PAGE_PROGRAMMED) {
		Synchronize([pcpage] {
			Form1->updateConsole(L">> Writing page: " + std::to_wstring(pcpage));
		});
		//ui.updateConsole(L">> Writing page: " + std::to_wstring(pcpage));

		address = static_cast<uint16_t>(pcpage << 5);
		memoryPage[1] = static_cast<uint8_t>(address >> 8);
		memoryPage[2] = static_cast<uint8_t>(address);

		// store page
		Form1->avrprog->writeMemoryPage(memoryPage);

		// wait until ready
		while(!Form1->avrprog->polling());
	}
    Synchronize([] {
		Form1->updateConsole(L">> Finished programming device...");
	});
	//ui.updateConsole(L">> Finished programming device...");
	//verifyFlash();
}
//---------------------------------------------------------------------------
