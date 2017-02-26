#pragma once

#include "common/Core.hpp"
#include "common/Dependency.hpp"

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

namespace MXNES {

class Window final : private Dependency<Core> {
public:
	Window();
	~Window();

	MXNES_DISABLE_ALTERNATE_CONSTRUCTORS(Window);

	HWND& get_hwnd();
	
	bool initialize();

	void pump_events();

private:
	//We need to pass these enum values to WIN32 as uints, so don't use an enum class
	//FYI make sure that these values don't conflict with the ones in common/resource.h
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
		WinApiWrapper<Handle_t, RetVal_t, _pfnDeallocator>& operator=(Handle_t handleArg) {
			_deallocate();
			_handle = handleArg;
			return *this;
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
	HINSTANCE _get_hinstance() const;

	static const char * const _WND_CLASS_NAME;
	static const DWORD _WINDOW_STYLES;
	static const DWORD _EX_WINDOW_STYLES;

	//Window::_thisWindow MUST be set before entering the message loop which will call this function!
	static LRESULT CALLBACK _wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	//A pointer to the Window object which _wnd_proc should use
	static Window *_thisWindow;
};

} //namespace MXNES