#include "SNES/ROM.hpp"

using namespace MXNES;
using namespace MXNES::SNES;

ROM::ROM() {

}

bool ROM::open_file(const std::string filePath) {

	std::ifstream fileStream(filePath, std::ios::in | std::ios::binary | std::ios::ate);
	if (!fileStream.is_open()) {
		Dependency<Core>::dep.logerr(std::string("Unable to open ROM file ") + filePath);
		Dependency<Core>::dep.alert_err("Failed to open ROM!");
		return false;
	}
	//Since we opened the file with std::ios::ate, tellg() will give us the file size
	_info.size = static_cast<u64>(fileStream.tellg());
	_info.hasAdditionalHeader = (_info.size & 0x3FF) == 0x200;

	std::unique_ptr<Header> pHeader(_find_valid_header(fileStream));

	fileStream.close();

	return true;
}

void ROM::reset() {

}

std::string ROM::to_string() const {
	return "";
}

void ROM::from_string(const std::string source) {
	MXNES_UNUSED_PARAMETER(source);
}

ROM::Header* ROM::_find_valid_header(std::ifstream &fileStream) const {
	u32 headerAddr;
	if (_check_header(fileStream, 0x7FC0)) { //i.e. LOROM
		headerAddr = 0x7FC0;
	}
	else if (_check_header(fileStream, 0xFFC0)) { //i.e. HIROM
		headerAddr = 0xFFC0;
	}
	else if (_info.size > 0x410000 && _check_header(fileStream, 0x40FFC0)) { //i.e. EXHIROM
		headerAddr = 0x40FFC0;
	}
	else {
		Dependency<Core>::dep.logerr("Failed to find a valid header; "
			"defaulting to LOROM header at ROM address 0x7FC0");
		headerAddr = 0x7FC0;
	}

	Header *pHeader = new Header;
	fileStream.seekg(headerAddr);
	fileStream.read(reinterpret_cast<char*>(pHeader->pBuff), _HEADERSIZ);

	return pHeader;
}

bool ROM::_check_header(std::ifstream &fileStream, const u32 romAddr) const {
	char headerBuff[_HEADERSIZ];

	fileStream.seekg(romAddr);
	fileStream.read(headerBuff, _HEADERSIZ);
	bool isHeaderValid = true;

	std::stringstream headerLoggingStringStream;
	headerLoggingStringStream << "Found valid header string: ";

	//If this is a legitimate header, then the string at romAddr shuld contain 20 
	//characters which should all be either an uppercase ASCII letter or a space
	for (int idx = 0; idx < 20; ++idx) {
		int currentChar = headerBuff[idx];
		if (!isupper(currentChar & 0xFF) && !isdigit(currentChar & 0xFF) && currentChar != ' ') {
			isHeaderValid = false;
			break;
		}
		else {
			headerLoggingStringStream << (char)currentChar;
		}
	}

	if (isHeaderValid) {
		Dependency<Core>::dep.logmsg(headerLoggingStringStream.str());
	}

	return isHeaderValid;
}

ROM::Info::Info() {

}

const std::string ROM::Info::to_string() const {
	return "";
}