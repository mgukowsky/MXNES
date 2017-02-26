#include "SNES/MMU.hpp"
#include "SNES/System.hpp"

using namespace MXNES;
using namespace MXNES::SNES;

#ifdef MXNES_TESTBUILD

#include <test/Testrunner.hpp>

int main(int argc, char **argv) {

	MXNES_UNUSED_PARAMETER(argc);
	MXNES_UNUSED_PARAMETER(argv);

	Testrunner tr;
	tr.run_test_suite();
	
	std::string s;
	std::cout << std::endl << "Press ENTER to exit...";
	std::getline(std::cin, s);

	return EXIT_SUCCESS;
}

#else

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	MXNES_UNUSED_PARAMETER(hInstance);
	MXNES_UNUSED_PARAMETER(hPrevInstance);
	MXNES_UNUSED_PARAMETER(lpCmdLine);
	MXNES_UNUSED_PARAMETER(nCmdShow);

	Core &appCore = Core::get_app_core();

	System sys;

	if (!sys.initialize())
		return EXIT_FAILURE;

	while (appCore.is_running()) {
		appCore.sleep_for_frame_delay();
		sys.process_events();
	}

	return EXIT_SUCCESS;
}

#endif //ifdef MXNES_TESTBUILD