#pragma once

#include "common/Registry.hpp"

#pragma warning(push, 0)
#include <chrono>
#include <ctime>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#pragma warning(pop)

namespace MXNES {

/**
 * The Core class encapsulates system-wide functionality such as logging and global state
 */
class Core {
public:
	Core();
	~Core();

	MXNES_DISABLE_ALTERNATE_CONSTRUCTORS(Core);
	
	bool is_running() const;
	void stop_running();

	/**
	 * @brief logmsg Writes to stdout
	 *
	 * @param msg The content to write to stdout
	 * @return none
	 */
	template<typename T>
	void logmsg(const T &msg) const {
		_log(msg, _logStream);
	}

	/**
	 * @brief logerr Writes to stderr
	 *
	 * @param msg The content to write to stderr
	 * @return none
	 */
	template<typename T>
	void logerr(const T &msg) const {
		_log(msg, _errStream);
	}

	/**
	 * @brief alert_msg Presents a message box dialog which halts all threads, then 
	 *		writes the text in the dialog to stdout
	 *
	 * @param msg The content to present to the user
	 * @return void
	 */
	void alert_msg(const char * const msg) const;

	/**
	 * @brief alert_msg Presents a message box dialog which halts all threads, then
	 *		writes the text in the dialog to stderr
	 *
	 * @param msg The content to present to the user
	 * @return void
	 */
	void alert_err(const char * const msg) const;

	/**
	 * @brief sleep Causes the current thread to sleep for a desired number of milliseconds
	 *
	 * @param milliseconds The number of milliseconds to sleep for
	 * @return none
	 */
	void sleep(const u32 milliseconds) const;

	/**
	 * @brief sleep_for_frame_delay Causes the thread to sleep for the number of milliseconds 
	 *		until the next frame needs to be drawn
	 *
	 * @return none
	 */
	void sleep_for_frame_delay();

	/**
	 * @brief get_app_core Returns a reference to the application-wide Core instance, and 
	 *		creates it if it does not yet exist
	 * @return A reference to the application-wide Core instance
	 */
	static Core& get_app_core();

private:

	std::ostream &_logStream, &_errStream;
	bool _isRunning;

	LARGE_INTEGER _previousTimestamp;

	const LARGE_INTEGER _SYSTEM_FREQUENCY;

	template<typename T>
	void _log(const T &msg, std::ostream &stream) const {
		//We need to lock here because localtime() modifies a static internal object
		//and, thus, it is not threadsafe
		std::lock_guard<std::mutex> lock(_mutex);
		char timeBuff[_STRFTIME_BUFFER_SIZE];
		std::time_t timeNow;
		time(&timeNow);
		strftime(timeBuff, _STRFTIME_BUFFER_SIZE, _STRFTIME_FORMAT_STRING, std::localtime(&timeNow));
		stream << timeBuff << " ----- " << msg << std::endl;
	}

	const LARGE_INTEGER _get_system_frequency() const;

	//mutable qualifier allows the mutex to be used in const member functions
	mutable std::mutex _mutex;

	static constexpr u16 _STRFTIME_BUFFER_SIZE = 128;
	static const char * const _STRFTIME_FORMAT_STRING;
};

} //namespace MXNES