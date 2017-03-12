#include "common/EventQueue.hpp"

using namespace MXNES;

EventQueue::EventQueue() 
	: _queue(nullptr), _readPtr(0), _writePtr(0) {

	_spinlock.clear();
	_queue = _memoryManager.allocate_managed_memory(_QUEUESIZ);
}

void EventQueue::register_event(const EventType EventType, void *data) {
#ifdef MXNES_USE_LOCKS
	while (_spinlock.test_and_set()) { /* spin */ }
	Event &currentEvent = *(_queue + _writePtr);
	if (currentEvent.type != EVENT_NULL) {
		//TODO: Don't override events in a locking scenario
		Dependency<Core>::dep.logerr("WARNING: Queue is full!!!");
	}
#else /* lockless */
	volatile Event &currentEvent = *(_queue + _writePtr);

	//If the event type is not null, then this means that the event at this slot in the queue 
	//has not yet been serviced; i.e. there is no room left in the queue
	while (currentEvent.type != EVENT_NULL) {
		Dependency<Core>::dep.logerr("Spinning while waiting for event to be serviced");
	}
#endif

	currentEvent.type = EventType;
	currentEvent.pData = data;
	++_writePtr;

#ifdef MXNES_USE_LOCKS
	_spinlock.clear();
#endif
}

EventQueue::Event EventQueue::service_event() {

#ifdef MXNES_USE_LOCKS
	while (_spinlock.test_and_set()) { /* spin */ }
#endif

	Event retVal = *(_queue + _readPtr);
	
	if (retVal.type != EVENT_NULL) {
		//Clear the event so that registration can continue in another thread
		(_queue + _readPtr)->type = EVENT_NULL;
		++_readPtr;
	}

#ifdef MXNES_USE_LOCKS
	_spinlock.clear();
#endif

	return retVal;
}