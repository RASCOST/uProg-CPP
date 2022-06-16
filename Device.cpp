#include "Device.h"

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

/************************
*
************************/
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

/****************************
*
****************************/
Device::Device(const std::wstring name) {

	architecture = std::make_unique<Architecture>();
	setDeviceName(name);
	readJSONFile();
}

void Device::setDeviceName(const std::wstring device) {
	name = device;
}

const std::wstring Device::getDeviceName() {
	return name;
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
		TJSONObject* fuseBytes = (TJSONObject*) TJSONObject::ParseJSONValue(device->GetValue("FuseBytes")->ToString());
		TJSONArray* extended = (TJSONArray*) TJSONObject::ParseJSONValue(fuseBytes->GetValue("extended")->ToString());
		architecture->fuseBytes->setExtended(extended);
		TJSONArray* high = (TJSONArray*) TJSONObject::ParseJSONValue(fuseBytes->GetValue("high")->ToString());
		architecture->fuseBytes->setHigh(high);
		TJSONArray* low = (TJSONArray*) TJSONObject::ParseJSONValue(fuseBytes->GetValue("low")->ToString());
		architecture->fuseBytes->setLow(low);

		// Signature Bytes Address
		TJSONArray* signatureBytesAddress = (TJSONArray*) TJSONObject::ParseJSONValue(device->GetValue("SignatureBytesAddress")->ToString());
		architecture->setSignatureBytesAddress(signatureBytesAddress);

		// Signature Bytes
		TJSONArray* signatureBytes = (TJSONArray*) TJSONObject::ParseJSONValue(device->GetValue("SignatureBytes")->ToString());
		architecture->setSignatureBytes(signatureBytes);

		delete jsonFile;
		delete device;
		delete lockBits;
		delete fuseBytes;
		delete extended;
		delete high;
		delete low;
		delete signatureBytesAddress;
		delete signatureBytes;

	}
	catch( const std::exception& e) {
         std::cout << e.what() << std::endl;
	}
}

