#include "Window.h"
#include "SNES\MMU.h"

using namespace MXNES;
using namespace MXNES::SNES;

#ifdef MXNES_TESTBUILD

#include "Testrunner.h"

int main(int argc, char **argv) {

	MXNES_UNUSED_PARAMETER(argc);
	MXNES_UNUSED_PARAMETER(argv);

	Testrunner tr;
	tr.run_test_suite();
	
	std::string s;
	std::cout << std::endl << "Press ENTER to exit...";
	std::getline(std::cin, s);

	return 0;
}

#else

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	MXNES_UNUSED_PARAMETER(hInstance);
	MXNES_UNUSED_PARAMETER(hPrevInstance);
	MXNES_UNUSED_PARAMETER(lpCmdLine);
	MXNES_UNUSED_PARAMETER(nCmdShow);

	Core &appCore = Core::get_app_core();

	Window wnd;

	if (!wnd.initialize())
		return 1;

	while (appCore.is_running()) {
		appCore.sleep_for_frame_delay();
		wnd.pump_events();
	}


	return 0;
}

#endif //ifdef MXNES_TESTBUILD