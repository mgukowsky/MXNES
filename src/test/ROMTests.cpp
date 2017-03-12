#ifdef MXNES_TESTBUILD

#include "SNES/ROM.hpp"
#include "test/Testrunner.hpp"

using namespace MXNES;
using namespace MXNES::SNES;

namespace MXNES_FILESCOPE {

	constexpr u8 MAGIC_VALUE = 0xAB;
	constexpr u16 MAGIC_ADDRESS = 0x0040;

} //namespace MXNES_FILESCOPE

void Testrunner::_rom_tests() {
	_start_test("ROM Behavior");

	ROM rom;
	//rom.open_file("");

	_end_test();
}

#endif //ifdef MXNES_TESTBUILD