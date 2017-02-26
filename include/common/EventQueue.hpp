#pragma once

#include "common/SmartMemoryManager.hpp"

namespace MXNES {

class EventQueue final : private Dependency<Core> {
public:

	enum EventType : u8 {
		EVENT_NULL = 0,
		EVENT_PICK_FILE
	};

	struct Event {
		u8 type;
		void *pData;
	};

	EventQueue();
	~EventQueue() = default;

	MXNES_DISABLE_ALTERNATE_CONSTRUCTORS(EventQueue);

	void register_event(const u8 EventType, void *data);
	Event service_event();

private:
	static constexpr u32 _QUEUESIZ = 0x10000;

	Event *_queue;
	volatile u16 _readPtr, _writePtr;
	SmartMemoryManager<Event> _memoryManager;
};

} //namespace MXNES