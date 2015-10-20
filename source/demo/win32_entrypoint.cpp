#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "Application.h"

#include <graphics/device.h>
#include <thread>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void MessageLoop();


INT WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine, 
	INT nCmdShow )
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"A window class";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.
	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"AppWindow",					// Window text
		WS_OVERLAPPEDWINDOW,            // Window style
										// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
		);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	MessageLoop();
	return 0;
}

void MessageLoop()
{
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static std::shared_ptr<Application> app;
	static std::unique_ptr<std::thread> mainthread;
	switch (uMsg)
	{
	case WM_CREATE:
		{
			// init the window
			auto device = graphics::make_device(hwnd);

			// initialize application class
			if ( device )
			{
				app = std::make_shared<Application>(std::move(device));
				std::weak_ptr<Application> weak_app(app);
				mainthread = std::make_unique<std::thread>(Application::app_thread, weak_app);
			}
			else
			{
				PostQuitMessage(0);
			}
		}
		// setup pixelformat
		return 0;
	case WM_CLOSE:
	case WM_DESTROY:
		app.reset();
		mainthread->join();
		mainthread.reset();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}