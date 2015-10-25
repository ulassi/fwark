#pragma once

#include <windows.h>
#include <memory>
#include <string>

class Win32Game_Impl;
class Win32Game
{
public:
	Win32Game(std::wstring name);
	~Win32Game();

	int run(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		PSTR lpCmdLine,
		INT nCmdShow );

	// customizations go here
	

private:
	std::unique_ptr<Win32Game_Impl> m_impl;
};