#pragma once

#include <memory>
#include <windows.h>
#include <core/types.h>
#include <core/scope.h>

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
		void clear(core::Real r, core::Real g, core::Real b);

		void begin_frame();
		void end_frame();
	private:
		std::unique_ptr<Device_Impl> m_impl;
	};
}
