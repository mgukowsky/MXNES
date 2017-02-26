#include "SNES/System.hpp"

using namespace MXNES;
using namespace MXNES::SNES;

System::System() {

}

System::~System() {

}

bool System::initialize() {
	return _wnd.initialize();
}

void System::process_events() {
	_wnd.pump_events();

	EventQueue::Event e;

	do {
		e = Dependency<EventQueueManager>::dep.windowQueue.service_event();

		switch (e.type) {
			case EventQueue::EVENT_NULL:
				break;

			case EventQueue::EVENT_PICK_FILE:
				_rom.open_file(_wnd.get_selected_file_path());
				break;
		}

	} while (e.type != EventQueue::EVENT_NULL);
}

void System::reset() {

}

std::string System::to_string() const {
	return "";
}

void System::from_string(const std::string source) {

}