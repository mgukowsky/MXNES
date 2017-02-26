#pragma once

#include "common/Component.hpp"
#include "common/EventQueueManager.hpp"
#include "common/Window.hpp"
#include "SNES/ROM.hpp"

namespace MXNES {
namespace SNES {

class System final : public Component, private Dependency<Core>,
	private Dependency<EventQueueManager> {

public:
	System();
	~System();

	MXNES_DISABLE_ALTERNATE_CONSTRUCTORS(System);

	bool initialize();
	void process_events();

	void reset();
	std::string to_string() const;
	void from_string(const std::string source);

private:
	Window _wnd;
	ROM _rom;

};

} //namespace SNES
} //namespace MXNES