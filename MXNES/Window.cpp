#include "Window.h"

using namespace MXNES;

const char * const Window::_WND_CLASS_NAME = "MXNESWND";

Window::Window()
	: _hwnd(nullptr) {

}

Window::~Window() {
	if (_hwnd != nullptr) {
		DestroyWindow(_hwnd);
	}
}

HWND Window::get_hwnd() const {
	return _hwnd;
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
		Dependency<Core>::dep.alert_err("Failed to register window class! This is likely a problem with the app itself. Shutting down...");
		return false;
	}

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = DEFAULT::APP_WIDTH;
	rect.bottom = DEFAULT::APP_HEIGHT;
	AdjustWindowRectEx(&rect, 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE, WS_EX_OVERLAPPEDWINDOW);

	_hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, _WND_CLASS_NAME, "MXNES", 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, 
		GetModuleHandle(nullptr), nullptr);

	if (_hwnd == nullptr) {
		Dependency<Core>::dep.alert_err("Failed to create window! This is likely a problem with the app itself. Shutting down...");
		return false;
	}

	ShowWindow(_hwnd, SW_SHOW);

	return true;
}

void Window::pump_events() const {
	MSG msg;

	while (PeekMessage(&msg, _hwnd, NULL, NULL, PM_REMOVE) != 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK Window::_wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
		case WM_CLOSE:
			Core::get_app_core().stop_running();
			break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}