#include "common/EventQueue.hpp"

using namespace MXNES;

EventQueue::EventQueue() 
	: _queue(nullptr), _readPtr(0), _writePtr(0) {

	_queue = _memoryManager.allocate_managed_memory(_QUEUESIZ);
}

void EventQueue::register_event(const u8 EventType, void *data) {
	volatile Event &currentEvent = *(_queue + _writePtr);
	while (currentEvent.type != EVENT_NULL) {
		Dependency<Core>::dep.logerr("Spinning while waiting for event to be serviced");
	}

	currentEvent.type = EventType;
	currentEvent.pData = data;
	++_writePtr;
}

EventQueue::Event EventQueue::service_event() {
	Event retVal = *(_queue + _readPtr);
	
	if (retVal.type != EVENT_NULL) {
		//Clear the event so that registration can continue in another thread
		(_queue + _readPtr)->type = EVENT_NULL;
		++_readPtr;
	}

	return retVal;
}