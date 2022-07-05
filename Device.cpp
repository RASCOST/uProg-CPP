#include "Device.h"

/******************************
*  Section Struct Fuse Bytes  *
******************************/
void FuseBytes::setExtended(TJSONArray* array) {
	for (uint8_t idx = 0; idx < array->Count; idx++)
		extended.push_back(array->Items[idx]->ToString().c_str());
}

void FuseBytes::setHigh(TJSONArray* array) {
	for (uint8_t idx = 0; idx < array->Count; idx++)
		high[idx] =  array->Items[idx]->ToString().c_str();
}

void FuseBytes::setLow(TJSONArray* array) {
	for (uint8_t idx = 0; idx < array->Count; idx++)
		low[idx] =  array->Items[idx]->ToString().c_str();
}

/*************************
*  Section Struct Flash  *
*************************/
void Flash::setFlashSize(std::wstring flashsize) {
	flashSize = flashsize;
}

void Flash::setPageSize(std::wstring pagesize) {
	pageSize = std::stoi(pagesize);
}

void Flash::setPcWord(TJSONArray* array) {
	for (uint8_t idx = 0; idx < array->Count; idx++)
		pcWord.push_back(std::stoi(array->Items[idx]->ToString().c_str()));
}

void Flash::setNumberPages(std::uint16_t pages) {
	numberPages = pages;
}

void Flash::setPcPage(TJSONArray* array) {
	for (uint8_t idx = 0; idx < array->Count; idx++)
		pcPage.push_back(std::stoi(array->Items[idx]->ToString().c_str()));
}

void Flash::setPcMsb(std::uint16_t pcmsb) {
	pcMsb = pcmsb;
}

std::vector<std::uint8_t> Flash::getPCWord() {
	return pcWord;
}

uint16_t Flash::getNumberPages() {
	return numberPages;
}

uint16_t Flash::getPageSize() {
	return pageSize;
}

/**************************
*  Section Struct EEPROM  *
**************************/
void EEPROM::setEepromSize(std::wstring eepromsize) {
	eepromSize = eepromsize;
}

void EEPROM::setPageSize(std::wstring pagesize) {
	pageSize = pagesize;
}

void EEPROM::setPcWord(TJSONArray* array) {
	for (uint8_t idx = 0; idx < array->Count; idx++)
		pcWord.push_back(std::stoi(array->Items[idx]->ToString().c_str()));
}

void EEPROM::setNumberPages(std::uint16_t pages) {
	numberPages = pages;
}

void EEPROM::setPcPage(TJSONArray* array) {
	for (uint8_t idx = 0; idx < array->Count; idx++)
		pcPage.push_back(std::stoi(array->Items[idx]->ToString().c_str()));
}

void EEPROM::setPcMsb(std::uint16_t pcmsb) {
	pcMsb = pcmsb;
}

/********************************
*  Section Struct Architecture  *
********************************/
void Architecture::setLockBits(TJSONArray* lockBits) {
	for (uint8_t lbit = 0; lbit < lockBits->Count; lbit++)
		LockBits.push_back(lockBits->Items[lbit]->ToString().c_str());
}

void Architecture::setSignatureBytesAddress(TJSONArray* address) {
	for (std::uint8_t position = 0; position < address->Count; position++)
		Signature_Bytes_Address[position] = std::stoi(address->Items[position]->ToString().c_str());
}

void Architecture::setSignatureBytes(TJSONArray* signature) {
	for (std::uint8_t position = 0; position < signature->Count; position++)
		Signature_Bytes[position] = std::stoi(signature->Items[position]->ToString().c_str());
}

/*************************
*  Section Class Device  *
*************************/
Device::Device() {

	architecture = std::make_unique<Architecture>();
	//setDeviceName(name);
	//readJSONFile();
}

void Device::setDeviceName(const std::wstring device) {
	name = device;
    readJSONFile();
}

const std::wstring Device::getDeviceName() {
	return name;
}

void Device::setFuseBytes(TJSONObject* object) {
	TJSONObject* fuseBytes = (TJSONObject*) TJSONObject::ParseJSONValue(object->GetValue("FuseBytes")->ToString());

	TJSONArray* extended = (TJSONArray*) TJSONObject::ParseJSONValue(fuseBytes->GetValue("extended")->ToString());
	architecture->fuseBytes->setExtended(extended);

	TJSONArray* high = (TJSONArray*) TJSONObject::ParseJSONValue(fuseBytes->GetValue("high")->ToString());
	architecture->fuseBytes->setHigh(high);

	TJSONArray* low = (TJSONArray*) TJSONObject::ParseJSONValue(fuseBytes->GetValue("low")->ToString());
	architecture->fuseBytes->setLow(low);

	delete fuseBytes;
	delete extended;
	delete high;
	delete low;
}

void Device::setFlash(TJSONObject* object) {
	TJSONObject* flash = (TJSONObject*) TJSONObject::ParseJSONValue(object->GetValue("Flash")->ToString());

	TJSONPair* flashsize = (TJSONPair*) TJSONObject::ParseJSONValue(flash->GetValue("FlashSize")->ToString());
	architecture->flash->setFlashSize(flashsize->ToString().c_str());

	TJSONPair* pagesize = (TJSONPair*) TJSONObject::ParseJSONValue(flash->GetValue("PageSize")->ToString());
	architecture->flash->setPageSize(pagesize->ToString().c_str());

	TJSONArray* pcword =  (TJSONArray*) TJSONObject::ParseJSONValue(flash->GetValue("PCWORD")->ToString());
	architecture->flash->setPcWord(pcword);

	TJSONPair* numberpages = (TJSONPair*) TJSONObject::ParseJSONValue(flash->GetValue("NumberPages")->ToString());
	architecture->flash->setNumberPages(std::stoi(numberpages->ToString().c_str()));

	TJSONArray* pcpage =  (TJSONArray*) TJSONObject::ParseJSONValue(flash->GetValue("PCPAGE")->ToString());
	architecture->flash->setPcPage(pcpage);

	TJSONPair* pcmsb = (TJSONPair*) TJSONObject::ParseJSONValue(flash->GetValue("PCMSB")->ToString());
	architecture->flash->setNumberPages(std::stoi(pcmsb->ToString().c_str()));

	delete flash;
	delete flashsize;
	delete pagesize;
	delete pcword;
	delete numberpages;
	delete pcpage;
	delete pcmsb;
}

void Device::setEEPROM(TJSONObject* object) {
	TJSONObject* eeprom = (TJSONObject*) TJSONObject::ParseJSONValue(object->GetValue("EEPROM")->ToString());

	TJSONPair* eepromsize = (TJSONPair*) TJSONObject::ParseJSONValue(eeprom->GetValue("EEPROMSize")->ToString());
	architecture->eeprom->setEepromSize(eepromsize->ToString().c_str());

	TJSONPair* pagesize = (TJSONPair*) TJSONObject::ParseJSONValue(eeprom->GetValue("PageSize")->ToString());
	architecture->eeprom->setPageSize(pagesize->ToString().c_str());

	TJSONArray* pcword =  (TJSONArray*) TJSONObject::ParseJSONValue(eeprom->GetValue("PCWORD")->ToString());
	architecture->eeprom->setPcWord(pcword);

	TJSONPair* numberpages = (TJSONPair*) TJSONObject::ParseJSONValue(eeprom->GetValue("NumberPages")->ToString());
	architecture->eeprom->setNumberPages(std::stoi(numberpages->ToString().c_str()));

	TJSONArray* pcpage =  (TJSONArray*) TJSONObject::ParseJSONValue(eeprom->GetValue("PCPAGE")->ToString());
	architecture->eeprom->setPcPage(pcpage);

	TJSONPair* pcmsb = (TJSONPair*) TJSONObject::ParseJSONValue(eeprom->GetValue("PCMSB")->ToString());
	architecture->eeprom->setNumberPages(std::stoi(pcmsb->ToString().c_str()));

	delete eeprom;
	delete eepromsize;
	delete pagesize;
	delete pcword;
	delete numberpages;
	delete pcpage;
	delete pcmsb;
}

void Device::readJSONFile() {
	try {
		std::unique_ptr<TStringStream> jsonStream(new TStringStream);
		std::wstring file = L"devices/" + getDeviceName() + L".json";
		jsonStream->LoadFromFile(file.c_str());

		TJSONObject* jsonFile = (TJSONObject*)TJSONObject::ParseJSONValue(jsonStream->DataString, false, true);
		TJSONObject* device = (TJSONObject*) TJSONObject::ParseJSONValue(jsonFile->GetValue(this->name.c_str())->ToString());

		// Lock Bits
		TJSONArray* lockBits = (TJSONArray*) TJSONObject::ParseJSONValue(device->GetValue("LockBits")->ToString());
		architecture->setLockBits(lockBits);

		// Fuse Bytes
		setFuseBytes(device);

		// Flash
		setFlash(device);

		// EEPROM
        setEEPROM(device);

		// Signature Bytes Address
		TJSONArray* signatureBytesAddress = (TJSONArray*) TJSONObject::ParseJSONValue(device->GetValue("SignatureBytesAddress")->ToString());
		architecture->setSignatureBytesAddress(signatureBytesAddress);

		// Signature Bytes
		TJSONArray* signatureBytes = (TJSONArray*) TJSONObject::ParseJSONValue(device->GetValue("SignatureBytes")->ToString());
		architecture->setSignatureBytes(signatureBytes);

		delete jsonFile;
		delete device;
		delete lockBits;
		delete signatureBytesAddress;
		delete signatureBytes;

	}
	catch( const std::exception& e) {
         std::cout << e.what() << std::endl;
	}
}

std::array<std::uint8_t, 3> Device::getSignatureBytesAddress() {

	return architecture->Signature_Bytes_Address;
}

std::array<std::uint8_t, 3> Device::getSignatureBytes() {

	return architecture->Signature_Bytes;
}

