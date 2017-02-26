#pragma once

#include "common/EventQueue.hpp"

namespace MXNES {

struct EventQueueManager {
	EventQueueManager() = default;
	~EventQueueManager() = default;

	MXNES_DISABLE_ALTERNATE_CONSTRUCTORS(EventQueueManager);

	EventQueue windowQueue;

};

} //namespace MXNES