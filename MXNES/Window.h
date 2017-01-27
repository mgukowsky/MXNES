#pragma once

#include "Core.h"
#include "Dependency.h"

namespace MXNES {

class Window : Dependency<Core> {
public:
	Window();
	~Window();

	MXNES_DISABLE_ALTERNATE_CONSTRUCTORS(Window);

	HWND get_hwnd() const;
	
	bool initialize();

	void pump_events() const;

private:
	HWND _hwnd;

	static const char * const _WND_CLASS_NAME;

	static LRESULT CALLBACK _wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

} //namespace MXNES