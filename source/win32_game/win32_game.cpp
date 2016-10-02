#include <win32_game\win32_game.h>
#include <win32_game\graphics_device.h>
#include <windows.h>

#include "Application.h"

static std::shared_ptr<Application> app;
static std::unique_ptr<std::thread> mainthread;
static Win32Game*					s_game = nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		{
			// init the window
			auto device = graphics::make_device(reinterpret_cast<ptrdiff_t>(hwnd));

			// initialize application class
			if (device && s_game)
			{
				app = std::make_shared<Application>(std::move(device));
				std::weak_ptr<Application> weak_app(app);
				mainthread = std::make_unique<std::thread>(Application::app_thread, weak_app);	
			}
			else
			{
				PostQuitMessage(0);
			}
			s_game->atLaunch();
		}
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

class Win32Game_Impl
{
public:
	Win32Game_Impl(std::wstring);
	
	std::wstring m_game_name;
	std::weak_ptr<Application> m_app;
	std::unique_ptr<systems::Loader> m_loader;
};
Win32Game_Impl::Win32Game_Impl(std::wstring name)
:	m_game_name(std::move(name))
{

}

Win32Game::Win32Game( std::wstring name )
	: m_impl(std::make_unique<Win32Game_Impl>(std::move(name)))
{

}

Win32Game::~Win32Game()
{

}

systems::Loader& Win32Game::getLoader()
{
	return *m_impl->m_loader;
}

int Win32Game::run(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine,
	INT nCmdShow)
{
	s_game = this;
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"UrbansWin32GameClass";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.
	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		m_impl->m_game_name.c_str(),			// Window text
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
		return -1;
	}

	

	ShowWindow(hwnd, nCmdShow);
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}