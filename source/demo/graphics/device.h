#pragma once

#include <memory>
#include <windows.h>

namespace graphics
{
	class Device;
	class Device_Impl;
	std::unique_ptr<Device> make_device(HWND);
	class Device
	{
		friend std::unique_ptr<Device> make_device(HWND);
	public:
		Device(HDC, HGLRC);
		~Device();
	private:
		std::unique_ptr<Device_Impl> m_impl;
	};
}
