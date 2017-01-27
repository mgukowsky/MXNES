#include "MMU.h"

using namespace MXNES;
using namespace MXNES::SNES;

MMU::MMU() 
	: _programBank(0), _dataBank(0), _pDirectPage(nullptr){
	_map_memory_lorom();
	reset();
}

void MMU::set_program_bank(const u8 newBank) {
	_programBank = MXNES_DECLARE_RVALUE(u16, (u16(newBank) << 4));
}

void MMU::set_data_bank(const u8 newBank) {
	_dataBank = MXNES_DECLARE_RVALUE(u16, (u16(newBank) << 4));
}

void MMU::map_memory(const MMU::MappingModel mappingModel) {
	_memoryManager.free_all();

	switch (mappingModel) {
		case MappingModel::LOROM:
			_map_memory_lorom();
			break;
		case MappingModel::HIROM:
		case MappingModel::LOROM_FASTROM:
		case MappingModel::HIROM_FASTROM:
		case MappingModel::EXLOROM:
		case MappingModel::EXHIROM:
		case MappingModel::MAPPING_MODEL_TOTAL:
		default:
			Dependency<Core>::dep.logerr("Invalid mapping model; defaulting to LOROM");
			_map_memory_lorom();
			break;
	}
}

/**
 * @brief reset zeroes out all pages which are not mirrors
 *
 * @return none
 */
void MMU::reset() {
	static constexpr u16 LIMIT = NUM_BANKS * NUM_BANKS_PER_PAGE;
	for (u16 pageIdx = 0; pageIdx < LIMIT; ++pageIdx) {
		Page &refPage = _pageTable[pageIdx];
		if (!refPage.isMapped) {
			refPage.reset();
		}
	}

	_programBank = 0;
	_dataBank = 0;
	_pDirectPage = _pageTable[0].pPageMem;
}

std::string MMU::to_string() const {
	return "";
}

void MMU::from_string(const std::string source) {

}

void MMU::_map_memory_lorom() {
	//First, create the WRAM banks so we can map them
	_allocate_page(0x7E0);
	_allocate_page(0x7E1);

	for (u16 bankIdx = 0; bankIdx < 0x40; ++bankIdx) {
		u16 bankOffset = MXNES_DECLARE_RVALUE(u16, (u16(bankIdx) << 4));

		//The first 2 pages in the bank are mirrors of 7E0 and 7E1, respectively
		_map_page(bankOffset, 0x7E0);
		_map_page(MXNES_DECLARE_RVALUE(u16, (bankOffset + 1)), 0x7E1);

		//The other 14 pages in the bank are not mirrors
		for (u16 pageIdx = 2; pageIdx < 0x10; ++pageIdx) {
			_allocate_page(MXNES_DECLARE_RVALUE(u16, (bankOffset + pageIdx)));
		}
	}

	//All subsequent pages until 0x7E0 are not mirrors
	for (u16 pageIdx = 0x400; pageIdx < 0x7E0; ++pageIdx) {
		_allocate_page(pageIdx);
	}
	
	//Ditto for pages between 0x7E2 and 0x800
	for (u16 pageIdx = 0x7E2; pageIdx < 0x800; ++pageIdx) {
		_allocate_page(pageIdx);
	}

	//Now, all pages until 0xFE0 are mirrors of the corresponding page
	//0x800 lower
	for (u16 pageIdx = 0x800; pageIdx < 0xFE0; ++pageIdx) {
		_map_page(pageIdx, MXNES_DECLARE_RVALUE(u16, (pageIdx - 0x800)));
	}

	//The remaining pages are not mirrors
	for (u16 pageIdx = 0xFE0; pageIdx < 0x1000; ++pageIdx) {
		_allocate_page(pageIdx);
	}
}

MMU::Page::Page() 
	: pPageMem(nullptr), isMapped(false) {}

void MMU::Page::reset() {
	if (pPageMem != nullptr && !isMapped) {
		UTIL::zero_memory(pPageMem, PAGESIZ);
	}
}

std::string MMU::Page::to_string() const {
	return "";
}

void MMU::Page::from_string(const std::string source) {

}