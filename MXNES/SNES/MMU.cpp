#include "MMU.h"

using namespace MXNES;
using namespace MXNES::SNES;

MMU::MMU()
 : _RAM(_smartMemoryManager.allocate_managed_memory(NUM_BANKS)) {
	reset();
}

MMU::~MMU(){}

void MMU::reset() {
	set_bank(0, BankType::BANK_PROGRAM);
	set_bank(0, BankType::BANK_DATA);
	set_direct_page(0);
}

std::string MMU::to_string() const {
	return "";
}

void MMU::from_string(const std::string source) {

}