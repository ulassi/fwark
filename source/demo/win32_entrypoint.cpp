#include <win32_game\win32_game.h>

INT WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine, 
	INT nCmdShow )
{
	Win32Game mygame(L"demo");
	return mygame.run(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

