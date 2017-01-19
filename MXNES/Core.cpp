#include "Core.h"

using namespace MXNES;

const char * const Core::_STRFTIME_FORMAT_STRING = "%c";

Core::Core() 
	: _logStream(std::cout), _errStream(std::cerr), _isRunning(true), 
		_SYSTEM_FREQUENCY(_get_system_frequency()) {

	QueryPerformanceCounter(&_previousTimestamp);
}

Core::~Core() {
	_isRunning = false;
}

bool Core::is_running() const {
	return _isRunning;
}

void Core::stop_running() {
	_isRunning = false;
}

void Core::alert_msg(const char * const msg) const {
	MessageBox(nullptr, msg, "MXNES Message", MB_OK | MB_ICONINFORMATION);
	logmsg(msg);
}

void Core::alert_err(const char * const msg) const {
	MessageBox(nullptr, msg, "MXNES Error!", MB_OK | MB_ICONERROR);
}

void Core::sleep(const u32 milliseconds) const {
	Sleep(milliseconds);
}

void Core::sleep_for_frame_delay() {
	u32 msToSleep;
	LARGE_INTEGER currentTimestamp, elapsedMicroseconds;
	
	QueryPerformanceCounter(&currentTimestamp);
	elapsedMicroseconds.QuadPart = currentTimestamp.QuadPart - _previousTimestamp.QuadPart;
	elapsedMicroseconds.QuadPart *= 1000000;
	elapsedMicroseconds.QuadPart /= _SYSTEM_FREQUENCY.QuadPart;

	msToSleep = std::lround(((DEFAULT::FRAMERATE_MS * 1000.0) 
		- elapsedMicroseconds.QuadPart) / 1000.0);

	//Events that block this thread, a long-running function, as well as the first invocation of this function, 
	//have the potential to cause elapsedMicroseconds to be greater than the framerate's millisecond period, which 
	//would cause msToSleep to wrap past 0 to an impossibly high value. To counter this, we detect such cases here
	//and simply prevent the thread from sleeping.
	if (msToSleep > (DEFAULT::FRAMERATE_MS)) {
		msToSleep = 0;
	}
	
	sleep(msToSleep);
	QueryPerformanceCounter(&_previousTimestamp);
}

Core& Core::get_app_core() {
	return Registry::retrieve_object<Core>();
}

const LARGE_INTEGER Core::_get_system_frequency() const {
	LARGE_INTEGER tmpFrequency;
	QueryPerformanceFrequency(&tmpFrequency);
	return tmpFrequency;
}
