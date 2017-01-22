#pragma once

#include "../Component.h"
#include "../Core.h"
#include "../SmartMemoryManager.h"

#include <cassert>

#define MXNES_ACCESS_MEMORY(ADDR, BANK_T) *(_pageTable[BANK_T | (ADDR >> 12)].pPageMem + (ADDR & 0xFFF))

namespace MXNES {
namespace SNES {

/**
 * Emulates the SNES MMU component
 */
class MMU final : public Dependency<Core>, public Component{
MXNES_DECLARE_TESTRUNNER_ACCESS;
public:
	static constexpr u32 PAGESIZ = 0x1000;
	static constexpr u32 NUM_BANKS = 0x100;
	static constexpr u32 NUM_BANKS_PER_PAGE = 0x10;

	enum class MappingModel {
		LOROM,
		HIROM,
		LOROM_FASTROM,
		HIROM_FASTROM,
		EXLOROM,
		EXHIROM,
		MAPPING_MODEL_TOTAL
	};

	MMU();
	~MMU() = default;

	void map_memory(const MappingModel mappingModel);

	FORCEINLINE u8 read_8_program(const u16 addr) {
		return MXNES_ACCESS_MEMORY(addr, _programBank);
	}

	FORCEINLINE u8 read_8_data(const u16 addr) {
		return MXNES_ACCESS_MEMORY(addr, _dataBank);
	}

	FORCEINLINE u8 read_8_direct_page(const u8 offset) {
		return *(_pDirectPage + offset);
	}

	FORCEINLINE u8 write_8_program(const u16 addr, const u8 val) {
		return MXNES_ACCESS_MEMORY(addr, _programBank) = val;
	}

	FORCEINLINE u8 write_8_data(const u16 addr, const u8 val) {
		return MXNES_ACCESS_MEMORY(addr, _dataBank) = val;
	}

	FORCEINLINE u8 write_8_direct_page(const u8 offset, const u8 val) {
		return *(_pDirectPage + offset) = val;
	}

	void set_program_bank(const u8 newBank);
	void set_data_bank(const u8 newBank);

	/**
	 * @brief reset Zeroes out all pages which are not mirrors
	 *
	 * @return none
	 */
	void reset();
	std::string to_string() const;
	void from_string(const std::string source);

private:
	struct Page final : public Component {
		Page();
		~Page() = default;
		u8 *pPageMem;
		bool isMapped;

		void reset();
		std::string to_string() const;
		void from_string(const std::string source);
	};

	void _map_memory_lorom();

	FORCEINLINE void _allocate_page(const u16 pageIdx) {
		Page &refPage = _pageTable[pageIdx];
		refPage.pPageMem = _memoryManager.allocate_managed_memory(PAGESIZ);
		refPage.isMapped = false;
	}

	//DO NOT CALL if mappedPageIdx is not yet allocated!
	FORCEINLINE void _map_page(const u16 pageIdx, const u16 mappedPageIdx) {
		Page &refPage = _pageTable[pageIdx];
		refPage.pPageMem = _pageTable[mappedPageIdx].pPageMem;
		assert(refPage.pPageMem != nullptr);
		refPage.isMapped = true;
	}

	Page _pageTable[NUM_BANKS * NUM_BANKS_PER_PAGE];
	u16 _programBank, _dataBank;
	u8 *_pDirectPage;

	SmartMemoryManager<u8> _memoryManager;

};
} //namespace SNES
} //namespace MXNES

#undef MXNES_ACCESS_MEMORY