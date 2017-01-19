#include "Window.h"
#include "SNES\MMU.h"

using namespace MXNES;
using namespace MXNES::SNES;

#ifdef MXNES_TESTBUILD

#include "Testrunner.h"

int main(int argc, char **argv) {
	Testrunner tr;
	tr.run_test_suite();
	
	std::string s;
	std::cout << std::endl << "Press ENTER to exit...";
	std::getline(std::cin, s);

	return 0;
}

#else

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Core &appCore = Core::get_app_core();

	Registry::register_object<Window>();
	Window &wnd = Registry::retrieve_object<Window>();
	MMU mmu;
	if (!wnd.initialize())
		return 1;

	while (appCore.is_running()) {
		appCore.sleep_for_frame_delay();
		wnd.pump_events();
	}

	Registry::destroy_object<Window>();

	return 0;
}

#endif //ifdef MXNES_TESTBUILD