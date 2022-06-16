//---------------------------------------------------------------------------

#pragma hdrstop

#include "IProgrammer.h"

#include "AVRProgrammer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
std::unique_ptr<IProgrammer> CreateProgrammer(IUIProgrammer& ui) {
	return std::make_unique<AVRProgrammer>(ui);
}
