#ifdef MXNES_TESTBUILD

#include "common/defs.hpp"
#include "SNES/MMU.hpp"
#include "test/Testrunner.hpp"

using namespace MXNES;
using namespace MXNES::SNES;

namespace MXNES_FILESCOPE {

	constexpr u8 MAGIC_VALUE = 0xAB;
	constexpr u16 MAGIC_ADDRESS = 0x0040;

} //namespace MXNES_FILESCOPE

void Testrunner::_mmu_tests() {
	_start_test("MMU Behavior");

	MMU mmu;
	mmu.write_8_program(MAGIC_ADDRESS, MAGIC_VALUE);
	_test_value(static_cast<u32>(mmu.read_8_program(MAGIC_ADDRESS)),
		static_cast<u32>(MAGIC_VALUE), "Writes and reads a byte");

	mmu.reset();
	_test_value(static_cast<u32>(mmu.read_8_program(MAGIC_ADDRESS)),
		static_cast<u32>(0), "Resets properly");

	_end_test();
}

#endif //ifdef MXNES_TESTBUILD