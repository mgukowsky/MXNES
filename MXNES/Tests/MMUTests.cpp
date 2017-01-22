#include "../Testrunner.h"
#include "../SNES/MMU.h"

using namespace MXNES;
using namespace MXNES::SNES;

void Testrunner::_mmu_tests() {
	_start_test("MMU Behavior");

	MMU mmu;
	mmu.write_8_program(0x0040, 0xAB);
	_test_value(static_cast<u32>(mmu.read_8_program(0x0040)), 
		static_cast<u32>(0xAB), "Writes and reads a byte");

	_end_test();
}