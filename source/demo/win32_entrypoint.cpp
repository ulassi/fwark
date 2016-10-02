#include <win32_game\win32_game.h>

class Demo : public Win32Game
{
public:
	Demo(std::wstring name) : Win32Game(std::move(name))
	{
	}

	void atLaunch() override
	{
		get<systems::Loader>().addPath("./");
		get<systems::Loader>().loadPackage("demo.pak");
	}
};

INT WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine, 
	INT nCmdShow )
{
	Demo mygame(L"demo");
	return mygame.run(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

