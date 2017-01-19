#pragma once

#include "../Component.h"
#include "../Core.h"
#include "../SmartMemoryManager.h"

namespace MXNES {
namespace SNES {

/**
 * Emulates the SNES MMU component
 */
class MMU final : public Component {
MXNES_DECLARE_TESTRUNNER_ACCESS;
public:
	static constexpr u32 PAGESIZ = 0x100;
	static constexpr u32 BANKSIZ = 0x10000;
	static constexpr u32 NUM_PAGES = 0x1000;
	static constexpr u32 NUM_BANKS = 0x100;

	enum class BankType {
		BANK_PROGRAM,
		BANK_DATA,
		BANK_TOTAL
	};

	MMU();
	~MMU();
	


	/**
	 * @brief set_bank changes an internal memory bank
	 *
	 * @param newVal The ID of the new bank value
	 * @param bankType Determines which bank to change
	 * @return none
	 */
	FORCEINLINE void set_bank(const u8 newVal, const BankType bankType) {
		Bank **ppBank;
		switch (bankType) {
		case BankType::BANK_PROGRAM:
			ppBank = &_pProgramBank;
			break;
		case BankType::BANK_DATA:
			ppBank = &_pDataBank;
			break;
		default:
			Core::get_app_core().logerr("Invalid bank type requested in MMU::set_bank; "
				"defaulting to program bank");
			ppBank = &_pProgramBank;
		}

		//TODO: Make sure the compiler figures out it can do this pointer arithmetic with
		//bit shifts rather than multiplication
		*ppBank = _RAM + newVal;
	}

	/**
	 * @brief set_direct_page changes the direct page to another page in the 0th memory bank
	 *
	 * @param newVal The ID of the new direct page value
	 * @return none
	 */
	FORCEINLINE void set_direct_page(const u8 newVal) {
		//TODO: See the TODO above in set_bank()
		_pDirectPage = reinterpret_cast<Page*>(_RAM) + newVal;
	}

	void reset();
	std::string to_string() const;
	void from_string(const std::string source);

private:

	/**
	 * RAM is made 256 contiguous banks
	 */
	struct Bank {
		u8 _bank[BANKSIZ];
	};

	/**
	 * Each bank is made up of 256 contiguous pages, each of which is
	 * made of up 256 bytes
	 */
	struct Page {
		u8 _page[PAGESIZ];
	};

	u16 _pageTable[NUM_PAGES];

	FORCEINLINE const u16 _map_address(const u16 rawAddr) {

	}

	/**
	 * Component which handles automatic memory management for this class
	 */
	SmartMemoryManager<Bank> _smartMemoryManager;

	/**
	 * N.B. _RAM is an immutable pointer to the base address of the 16MB of total memory
	 */
	Bank *_pProgramBank, *_pDataBank, *const _RAM;
	Page *_pDirectPage;

};
} //namespace SNES
} //namespace MXNES