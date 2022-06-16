//---------------------------------------------------------------------------

#ifndef IProgrammerH
#define IProgrammerH
//---------------------------------------------------------------------------

#include <memory>
#include <string>

enum class FUSE_BYTES { LOW, HIGH, EXTENDED };

class IUIProgrammer {
public:
	virtual void updateConsole(const std::wstring& message) = 0;
    virtual void updateLockBits() = 0;
};

class IProgrammer {
public:
	//virtual ~IProgrammer();
    virtual bool programmingEnable() = 0;
	virtual void openFile(std::string fileName) = 0;
	virtual void readFsBits(FUSE_BYTES byte) = 0;
	virtual void writeFsBits() = 0;
	virtual void readLBits() = 0;
	virtual void writeLBits() = 0;
	virtual void readFlash() = 0;
	virtual void writeFlash() = 0;
	virtual void verifyFlash() = 0;
	virtual void readEeprom() = 0;
	virtual void writeEeprom() = 0;
};

std::unique_ptr<IProgrammer> CreateProgrammer(IUIProgrammer& ui);
#endif
