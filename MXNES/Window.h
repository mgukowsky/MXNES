#pragma once

#include "Core.h"
#include "Dependency.h"

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

namespace MXNES {

class Window : Dependency<Core> {
public:
	Window();
	~Window();

	MXNES_DISABLE_ALTERNATE_CONSTRUCTORS(Window);

	HWND get_hwnd() const;
	
	bool initialize();

	void pump_events();

private:
	//We need to pass these enum values to WIN32 as uints, so don't use an enum class
	enum MenuID {
		IDM_FILE_OPEN = 1,
		IDM_FILE_CLOSE,
		IDM_FILE_EXIT,
		IDM_HELP_ABOUT
	};

	template<typename Handle_t, typename RetVal_t, RetVal_t(WINAPI *_pfnDeallocator)(Handle_t)>
	class WinApiWrapper {
	public:
		WinApiWrapper() : _handle(nullptr) {}
		WinApiWrapper(Handle_t handleArg) : _handle(handleArg) {}
		~WinApiWrapper() { 
			_deallocate();
		}
		void operator=(Handle_t handleArg) {
			_deallocate();
			_handle = handleArg;
		}
		Handle_t& get() { return _handle; }
		bool is_nullptr() { return _handle == nullptr; }
	private:
		Handle_t _handle;
		void _deallocate() {
			if (_handle != nullptr)
				(_pfnDeallocator(_handle));
		}
	};

	WinApiWrapper<HWND, BOOL, DestroyWindow> _hwndWrapper;

	typedef WinApiWrapper<HMENU, BOOL, DestroyMenu> HMenuWrapper;
	std::vector<HMenuWrapper> _hMenus;
	std::string _currentFile;

	bool _create_menu();
	const std::string _pick_file();

	HWND _hwnd;

	static const char * const _WND_CLASS_NAME;
	static const DWORD _WINDOW_STYLES;
	static const DWORD _EX_WINDOW_STYLES;

	//Window::thisWindow MUST be set before entering the message loop which will call this function!
	static LRESULT CALLBACK _wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	//A pointer to the Window object which _wnd_proc should use
	static Window *thisWindow;
};

} //namespace MXNES