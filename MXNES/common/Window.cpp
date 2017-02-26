#include <common/Window.hpp>

using namespace MXNES;

const char * const Window::_WND_CLASS_NAME = "MXNESWND";
const DWORD Window::_WINDOW_STYLES = WS_OVERLAPPED | WS_CAPTION | 
	WS_SYSMENU | WS_MINIMIZEBOX;
const DWORD Window::_EX_WINDOW_STYLES = WS_EX_OVERLAPPEDWINDOW;

Window* Window::_thisWindow = nullptr;

Window::Window() {

}

Window::~Window() {

}

HWND& Window::get_hwnd() {
	return _hwndWrapper.get();
}

bool Window::initialize() {

	WNDCLASSEX wcex;
	UTIL::zero_memory(&wcex);

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = CreateSolidBrush(RGB(0x00, 0xFF, 0x00));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hInstance = GetModuleHandle(nullptr);
	wcex.lpfnWndProc = _wnd_proc;
	wcex.lpszClassName = _WND_CLASS_NAME;
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	if (!RegisterClassEx(&wcex)) {
		Dependency<Core>::dep.alert_err("Failed to register window class! "
			"This is likely a problem with the app itself. Shutting down...");
		return false;
	}

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = DEFAULT::APP_WIDTH;
	rect.bottom = DEFAULT::APP_HEIGHT;
	AdjustWindowRectEx(&rect, 
		_WINDOW_STYLES, TRUE, _EX_WINDOW_STYLES);

	_hwndWrapper = CreateWindowEx(_EX_WINDOW_STYLES, _WND_CLASS_NAME, "MXNES",
		_WINDOW_STYLES, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, 
		GetModuleHandle(nullptr), nullptr);

	if (_hwndWrapper.is_nullptr()) {
		Dependency<Core>::dep.alert_err("Failed to create window! This is likely "
			"a problem with the app itself. Shutting down...");
		return false;
	}

	if (!_create_menu())
		return false;

	ShowWindow(_hwndWrapper.get(), SW_SHOW);

	return true;
}

void Window::pump_events() {
	MSG msg;

	//Make this window the current context
	//TODO: Should we add a lock here to handle multiple windows in different 
	//threads accessing the message loop below?
	_thisWindow = this;

	while (PeekMessage(&msg, _hwndWrapper.get(), NULL, NULL, PM_REMOVE) != 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

bool Window::_create_menu() {
	_hMenus.insert(_hMenus.begin(), 3, nullptr);
	for (auto &hMenu : _hMenus) {
		hMenu = CreateMenu();
	}

	auto hMenuFile = _hMenus.begin();
	auto hMenuHelp = hMenuFile + 1;
	auto hMenuBar = hMenuHelp + 1;

	if (hMenuFile->is_nullptr() || hMenuHelp->is_nullptr() || hMenuBar->is_nullptr()) {
		Dependency<Core>::dep.alert_err("Failed to create app menu! "
			"This is likely a problem with the app itself. Shutting down...");
		return false;
	}

	AppendMenu(hMenuFile->get(), MF_STRING, IDM_FILE_OPEN, "&Open ROM...");
	AppendMenu(hMenuFile->get(), MF_STRING | MF_GRAYED, IDM_FILE_CLOSE, "&Close ROM...");
	AppendMenu(hMenuFile->get(), MF_SEPARATOR, NULL, nullptr);
	AppendMenu(hMenuFile->get(), MF_STRING, IDM_FILE_EXIT, "&Exit");

	AppendMenu(hMenuHelp->get(), MF_STRING, IDM_HELP_ABOUT, "&About");

	AppendMenu(hMenuBar->get(), MF_POPUP, reinterpret_cast<UINT_PTR>(hMenuFile->get()), "&File");
	AppendMenu(hMenuBar->get(), MF_POPUP, reinterpret_cast<UINT_PTR>(hMenuHelp->get()), "&Help");

	SetMenu(_hwndWrapper.get(), hMenuBar->get());

	return true;
}

const std::string Window::_pick_file() {

	OPENFILENAME ofn;
	char pathBuff[BUFSIZ];
	UTIL::zero_memory(&ofn);
	UTIL::zero_memory(&pathBuff);

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = _hwndWrapper.get();
	ofn.hInstance = GetModuleHandle(NULL);
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0";
	ofn.lpstrFile = pathBuff;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "txt";

	if (!GetOpenFileName(&ofn)) {
		Dependency<Core>::dep.alert_err("Failed to open file!");
		return "";
	}
	else {
		Dependency<Core>::dep.logmsg(std::string("Selected file: \"") + pathBuff + "\"");
		return pathBuff;
	}
}

//Window::_thisWindow MUST be set before entering the message loop which will call this function!
LRESULT CALLBACK Window::_wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDM_FILE_OPEN:
					_thisWindow->_currentFile = _thisWindow->_pick_file();
					break;
				case IDM_FILE_EXIT:
					SendMessage(hwnd, WM_CLOSE, NULL, NULL);
					break;
				case IDM_HELP_ABOUT:
					MessageBeep(MB_ICONINFORMATION);
					MessageBox(nullptr, "Thanks for using MXNES, an emulator for the NES and SNES consoles, "
						"by Matt Gukowsky \xA9 2016", "About", MB_OK | MB_ICONQUESTION | MB_TASKMODAL);
			}
			break;

		case WM_CLOSE:
			_thisWindow->Dependency<Core>::dep.stop_running();
			break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}